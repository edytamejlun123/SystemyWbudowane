/*
 * File:   newmainXC16.c
 * Author: local
 *
 * Created on April 3, 2024, 3:07 PM
 * 
 * Automatyczne zliczanie w gór? i w dó?.
 * 
 */

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
#pragma config JTAGEN = OFF  

#include "xc.h"
#include <libpic30.h>


unsigned portValue = 0;

char prev86 = 0, current86 = 0, prev87 = 0, current87 = 0, program=0; 

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    if(program ==0){
        portValue++;
    }
    if(program == 1){
        portValue--;
    }
    LATA = portValue;
    _T1IF = 0; //interrupt flag - na koniec przerwania ustawiamy na zero = resetujemy licznik :) 
}

int main(void) {
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    T1CON = 0x8020; //binarna wersja
    PR1 = 0xFFFF;
    _T1IE = 1;
    _T1IP = 1;
    
    while(1){
        prev86 = PORTDbits.RD6;
        prev87 = PORTDbits.RD7;
        __delay32(2000);
        current86 = PORTDbits.RD6;
        current87 = PORTDbits.RD7;
        if(current86-prev86 == -1){
            program = 0;
        }
        if(current87-prev87 == -1){
            program = 1;
        }
    }
    
    return 0;
}
