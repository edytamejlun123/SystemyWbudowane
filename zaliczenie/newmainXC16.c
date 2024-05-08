/*
 * File:   zad1.c
 * Author: local
 *
 * Created on April 17, 2024, 3:49 PM
 */


// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRIPLL           // Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL))
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
#include <math.h>

unsigned portValue = 0;
char currentS6 = 0, currentS7= 0, prevS6 = 0, prevS7 = 0;

void licznikBinarny1(){
    unsigned portValue = 0;
    while(1){
        LATA= portValue; 
        portValue++;
        __delay32(100000);
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
        if(prevS6 - currentS6 == 1){
            licznikBinarny2();
        }
        
    }
}
void licznikBinarny2(){
    unsigned portValue = 255;
    while(1){
        LATA= portValue; 
        portValue--;
        __delay32(100000);
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
       if (prevS7 - currentS7 == 1)
        {
            licznikBinarny1();
        }
             
    }
}

void wezyk(){
    unsigned portValue = 0;
    while(1){
        LATA= pow(2, portValue) *7; 
        portValue++;
        __delay32(100000);
        
    }
}

int main(void) {
   /*  while(1){
        TRISAbits.TRISA0=0;
        PORTAbits.RA0 = 1;
    }*/
    
   unsigned portValue = 0;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    char currentS6 = 0, currentS7= 0, prevS6 = 0, prevS7 = 0;
    
    /*while(1){  
      //  licznikBinarny2();
        prevS6 = PORTDbits.RD6;
        __delay32(15000);
        currentS6 = PORTDbits.RD6;
        if(currentS6 - prevS6 == 1){
            licznikBinarny2();
        }
        prevS7 = PORTDbits.RD7;
        __delay32(15000);
        currentS7 = PORTDbits.RD7;
        if(currentS7 - prevS7 == 1){
            licznikBinarny1();
            
        }
        
       
    }*/
    int licznik = 0;
    while(1){
        LATA= pow(2, portValue)*7; 
       
        if(licznik < 6){
             portValue++;
             licznik++;
        }
        elif(licznik>6 && licznik <11){
            portValue--;
            licznik++;
        }
        if(licznik==11){
            licznik=0;
        }
        __delay32(5000000);
        
    }
    
    return 0;
}

/*1+2+4 = 7 /7= 1 -> 2^0
2+4+8 = 14 /7 = 2 -> 2^1
4+8+16 = 28 /7 = 4 -> 2^2
8+16+32 = 56/7 = 8 -> 2^3
16+32+64 = 112/7 = 16 -> 2^4
32+64 + 128 = 224/7 = 32 -> 2^5

*/