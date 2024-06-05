// CONFIG2
#pragma config POSCMOD = NONE             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRC           // Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL))
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)
 
// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)
 
#include "xc.h"
#include <string.h>
#include <libpic30.h>
 
#define FCY       4000000UL
#define LCD_E     LATDbits.LATD4
#define LCD_RW    LATDbits.LATD5
#define LCD_RS    LATBbits.LATB15
#define LCD_DATA  LATE
 
#define BTN_A (PORTDbits.RD6 == 0)
#define BTN_B (PORTDbits.RD7 == 0)
#define BTN_C (PORTAbits.RA7 == 0)
#define BTN_D (PORTDbits.RD13 == 0)
 
#define LCD_CLEAR               0b1
#define LCD_CR                  0b10
#define _LCD_INPUTMODE(ID, S)   (0b100 + (ID<<1) + S)
#define _LCD_SCREENMODE(D,C,B)  (0b1000 + (D<<2) + (C<<1) + B)
#define _LCD_CONFIG(DL,N,F)     (0b100000 + (DL<<4) + (N<<3) + (F<<2))
#define LCD_ON                  _LCD_SCREENMODE(1,0,0)
#define LCD_OFF                 _LCD_SCREENMODE(0,0,0)
#define LCD_CONFIG              _LCD_CONFIG(1,1,0)
#define LCD_CUST_CHAR           0x40
#define LCD_SHIFT_R             0x1D
#define LCD_CURSORPOS(P)        (0b10000000 + (P&0b1111111))
 
//synchronizowany delay
//obecna suma czasu
long synctime = 0;
void sync_to(long t){
    //czekaj okreslony czas biorac pod uwage poprzednie delaye
    if (t > synctime){
        __delay32(t-synctime);
    }
    synctime = 0;
}
//delaye ktore dodaja do sumowanego czasu:
void sleep_sync(long a){
    __delay32(a);
    synctime += a;
}
void sleep_sync_us(long us){
    sleep_sync(FCY/1000000*us);
}
void sleep_sync_ms(long us){
    sleep_sync(FCY/1000*us);
}
 
//funkcja zamiany liczby na znaki do tablicy docelowej target
void toString(char* target, int digits, unsigned n){
    for (int x = 0; x < digits; x++){
        target[digits-1-x] = '0' + (n%10);
        n /= 10;
    }
}
 
//funkcje lcd:
void LCD_sendCommand(uint8_t command){
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_E = 1;
    LCD_DATA = command;
    sleep_sync_us(50);
    LCD_E = 0;
    sleep_sync_us(50);
}
void LCD_sendData(unsigned char data){
    LCD_RW = 0;
    LCD_RS = 1;
    LCD_E = 1;
    LCD_DATA = data;
    sleep_sync_us(50);
    LCD_E = 0;
    sleep_sync_us(50);
}
//wypisanie tekstu na ekran w okreslonej linii
void LCD_print(char* text, int line){
    int c = 0;
    if (line != 2){
        LCD_sendCommand(LCD_CURSORPOS(line == 0 ? 0x00 : 0x40));
    }
    while (*text && c++ < 0x10){
        LCD_sendData(*(text++));
    }
}
void LCD_init(){
    sleep_sync_ms(20);
    LCD_sendCommand(LCD_CONFIG);
    LCD_sendCommand(LCD_ON);
    LCD_sendCommand(LCD_CLEAR);
    sleep_sync_ms(2);
}
 
 
int w = 0;
int time = 30;
//obecny stan
unsigned state = 0;
 
//stan 0: ustawianie godziny i mocy
void state0(){
    unsigned p_val = ADC1BUF0 / 64; //8 ró?nych warto?ci
    w = 500 + (p_val * 50);
    //tekst (minuta:godzina, moc) na ekran lcd
    char l1[0x10];
    memset(l1, ' ', 0x10);
    unsigned min = time/60;
    unsigned sec = time % 60;
    toString(l1+1, 2, min);
    l1[4] = ':';
    toString(l1+5, 2, sec);
    toString(l1+10, 4, w);
    l1[14] = 'W';
    LCD_print(l1, 0);
    LCD_print("  -   +   START   ", 1);
    //przyciski
    //przycisk A: czas-
    if (BTN_A){
        if (time > 15){
            time -= 15;
            sleep_sync_ms(250);
        }
    }
    //przycisk B: czas+
    if (BTN_B) {
        if (time < 600) {
            time += 15;
            sleep_sync_ms(250);
        }
    }
    //przycisk C: start
    if (BTN_C) {
        state = 1;
        //reset licznika synchronizacji 
        sync_to(1);
    }
}
//stan 1: stan dzialania
void state1(){
    //powrót do ekranu startowego po czasie lub przycisku stop
    if (time <= 0 || BTN_D){
        time = 30;
        state = 0;
        return;
    }
    time--;
    //zamiana czasu na tekst
    char l1[0x10];
    memset(l1, ' ', 0x10);
    unsigned min = time/60;
    unsigned sec = time % 60;
    toString(l1+1, 2, min);
    l1[4] = ':';
    toString(l1+5, 2, sec);
    toString(l1+10, 4, w);
    l1[14] = 'W';
    LCD_print(l1, 0);
    LCD_print("           STOP", 1);
    //synch. do ~sekundy
    sync_to(FCY);
}
 
int main(void) {
    //konfiguracja i/o
    TRISA = 0b10000000;
    TRISB = 0x7FFF; //potencjometr, czujnik temperatury
    TRISD = 0b10000011000000;
    TRISE = 0;
    AD1CON1 = 0x80E4;   //konfiguracja
    AD1CON2 = 0x0404;   //kana?y, wielo czy jednokana?owo
    AD1CON3 = 0x0F00;
    AD1CHS = 0;         //wybór fizycznego urz?dzenia
    AD1CSSL = 0x0020;
    LCD_init();
    //tablica stanow
    void (*states[])() = {
        &state0,
        &state1
    };
    //p?tla wykonujaca obecny stan
    while (1){
        states[state]();
    }
    return 0;
}