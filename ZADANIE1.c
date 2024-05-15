/*
 * File:   newmainXC16.c
 * Author: local
 *
 * Created on 15 maja 2024, 15:15
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
#include <time.h>

#define BIT_VALUE(value, noBit) (value >> noBit) & 1


unsigned portValue = 0;
char currentS6 = 0, currentS7= 0, prevS6 = 0, prevS7 = 0;

void licznikBinarny1(){
    unsigned portValue = 0;
    while(1){
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
        LATA= portValue; 
        portValue++;
        __delay32(900000);
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
        if(prevS6 - currentS6 == 1){
            wezyk();
        }
        if(prevS7 - currentS7 == 1){
            
            licznikBinarny2();
        }
        
    }
}
void licznikBinarny2(){
    unsigned portValue = 255;
    while(1){
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
        LATA= portValue; 
        portValue--;
        __delay32(900000);
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
       if (prevS7 - currentS7 == 1)
        {
           wezyk();
            
        }
         if (prevS6 - currentS6 == 1)
        {
            licznikBinarny1();
        }
             
    }
}

void Grey2(){
    unsigned portValue = 0;
    while(1){
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
        LATA= (portValue>>1)^portValue; 
        portValue++;
        __delay32(900000);
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
       if (prevS7 - currentS7 == 1)
        {
           wezyk();
            
        }
         if (prevS6 - currentS6 == 1)
        {
            licznikBinarny1();
        }
             
    }
}

void Grey1(){
    unsigned portValue = 255;
    while(1){
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
        LATA= (portValue>>1)^portValue; 
        portValue--;
        __delay32(900000);
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
       if (prevS7 - currentS7 == 1)
        {
           wezyk();
            
        }
         if (prevS6 - currentS6 == 1)
        {
            licznikBinarny1();
        }
             
    }
}

void BCD1(){
    unsigned portValueL = 0;
    unsigned portValueP = 0;
    while(1){
        if(portValueP == 10){
            portValueL +=1; 
            portValueP = 0;
        }
        if(portValueL == 9 && portValue == 9){
            portValueL =0; 
            portValueP = 0;
        }
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
        LATA= (portValueL<<4)+portValueP; 
        portValueP++;
       
        __delay32(4000000);
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
       if (prevS7 - currentS7 == 1)
        {
           wezyk();
            
        }
         if (prevS6 - currentS6 == 1)
        {
            licznikBinarny1();
        }
             
    }
}

void BCD2(){
    unsigned portValueL = 9;
    unsigned portValueP = 9;
    while(1){
        if(portValueP == 0){
            portValueL -=1; 
            portValueP = 9;
        }
        if(portValueL == 0 && portValue == 0){
            portValueL =9; 
            portValueP = 9;
        }
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
        LATA= (portValueL<<4)+portValueP; 
        portValueP--;
       
        __delay32(4000000);
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
       if (prevS7 - currentS7 == 1)
        {
           wezyk();
            
        }
         if (prevS6 - currentS6 == 1)
        {
            licznikBinarny1();
        }
             
    }
}

void wezyk(){
    unsigned portValue = 0;
    int licznik = 0;
    while(1){
        prevS7 = PORTDbits.RD7; 
        prevS6 = PORTDbits.RD6;
       //LATA= pow(2, portValue)*0b111; 
        LATA = 0b111 << portValue;
        if(licznik < 5){
             portValue++;
             licznik++;
        }
        else if(licznik>=5 && licznik <10){
            portValue--;
            licznik++;
        }
        if(licznik==10){
            licznik=0;
        }
        __delay32(900000);
        
        __delay32(150000);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
       if (prevS7 - currentS7 == 1)
        {
            licznikBinarny1();
        }
         if (prevS6 - currentS6 == 1)
        {
            licznikBinarny2();
        }
        
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
   /* 
    while(1){  
      //  licznikBinarny2();
        prevS6 = PORTDbits.RD6;
        __delay32(200000);
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
    
    while(1){
        int seed=0; 
        int val= BIT_VALUE(seed, 0);
        LATA = val;
        __delay32(900000);
        for (int j=1; j<70; j++){
        for( int i=1; i<6; i++){
            if(BIT_VALUE(51, i) == 1){
                val = val ^ BIT_VALUE(seed,i);
            }
            __delay32(900000);
        }

        seed = (seed>>1) + (val*32);
        j++;
        } }
    
   
    return 0;
}

/*1+2+4 = 7 /7= 1 -> 2^0
2+4+8 = 14 /7 = 2 -> 2^1
4+8+16 = 28 /7 = 4 -> 2^2
8+16+32 = 56/7 = 8 -> 2^3
16+32+64 = 112/7 = 16 -> 2^4
32+64 + 128 = 224/7 = 32 -> 2^5

*/
