#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "buttons.h"

#define LICZBA_SEKUND (60) // Ca?kowity pocz?tkowy czas dla ka?dego gracza w sekundach

volatile unsigned int pozostalyCzas = LICZBA_SEKUND;

void opoznienie_ms(unsigned long milisekundy) {
    while (milisekundy--) {
        __delay32(4000); // Zak?adaj?c 8 MHz FCY (8 MIPS)
    }
}

void odliczanieStartowe() {
    char buforWyswietlacza[32];
    for (int i = 3; i > 0; i--) {
        LCD_ClearScreen();
        LCD_PutString("START GRY ZA:", strlen("START GRY ZA:"));
        sprintf(buforWyswietlacza, " %d", i);
        LCD_PutString(buforWyswietlacza, strlen(buforWyswietlacza));
        opoznienie_ms(1000);
    }
}

int main(void) {
    char buforWyswietlacza[32];
    LCD_Initialize();
    unsigned int bialePozostalyCzas = pozostalyCzas;
    unsigned int czarnePozostalyCzas = pozostalyCzas;
    int aktualnaFaza = 3; // 3 - wybór rozpoczynaj?cego, 1 - tura bia?ych, 2 - tura czarnych, 0 - koniec gry

    while (1) {
        // Sprawdzamy przyciski w ka?dej iteracji p?tli g?ównej
        if (BUTTON_IsPressed(BUTTON_S5)) {
            aktualnaFaza = 0; // Mat
            LCD_ClearScreen();
            LCD_PutString("MAT", strlen("MAT")); 
            opoznienie_ms(2000); 
            bialePozostalyCzas = pozostalyCzas;
            czarnePozostalyCzas = pozostalyCzas;
            aktualnaFaza = 3; 
            continue; 
        }

        if (BUTTON_IsPressed(BUTTON_S3) && BUTTON_IsPressed(BUTTON_S4)) {
            aktualnaFaza = 0; // Remis
            LCD_ClearScreen();
            LCD_PutString("REMIS", strlen("REMIS")); 
            opoznienie_ms(2000); 
            bialePozostalyCzas = pozostalyCzas;
            czarnePozostalyCzas = pozostalyCzas;
            aktualnaFaza = 3; 
            continue; 
        }

        switch (aktualnaFaza) {
            case 0: // Koniec czasu
                LCD_ClearScreen();
                if (bialePozostalyCzas < 1) {
                    LCD_PutString("Czrany wygrywa", strlen("Czrany wygrywa"));
                } else if (czarnePozostalyCzas < 1) {
                    LCD_PutString("Bialy wygrywa", strlen("Bialy wygrywa"));
                }
                opoznienie_ms(10000); 
                bialePozostalyCzas = pozostalyCzas; 
                czarnePozostalyCzas = pozostalyCzas; 
                aktualnaFaza = 3;  
                break;

            case 1: // Tura bia?ych 
                if (BUTTON_IsPressed(BUTTON_S4)) { 
                    aktualnaFaza = 2;
                    break;
                }
                sprintf(buforWyswietlacza, "%02u:%02u B", bialePozostalyCzas / 60, bialePozostalyCzas % 60); // Wy?wietlanie czasu bia?ych
                LCD_ClearScreen();
                LCD_PutString(buforWyswietlacza, strlen(buforWyswietlacza));
                LCD_PutString("\n", 1);
                sprintf(buforWyswietlacza, "%02u:%02u CZ", czarnePozostalyCzas / 60, czarnePozostalyCzas % 60); // Wy?wietlanie czasu czarnych
                LCD_PutString(buforWyswietlacza, strlen(buforWyswietlacza));
                opoznienie_ms(1000);
                bialePozostalyCzas--; // Dekrementacja czasu bia?ych
                if (bialePozostalyCzas < 1) {
                    aktualnaFaza = 0;
                }
                break;

            case 2: // Tura czarnych 
                if (BUTTON_IsPressed(BUTTON_S3)) { 
                    aktualnaFaza = 1;
                    break;
                }
                sprintf(buforWyswietlacza, "%02u:%02u CZ", czarnePozostalyCzas / 60, czarnePozostalyCzas % 60); // Wy?wietlanie czasu czarnych
                LCD_ClearScreen();
                LCD_PutString(buforWyswietlacza, strlen(buforWyswietlacza));
                LCD_PutString("\n", 1);
                sprintf(buforWyswietlacza, "%02u:%02u B", bialePozostalyCzas / 60, bialePozostalyCzas % 60); // Wy?wietlanie czasu bia?ych
                LCD_PutString(buforWyswietlacza, strlen(buforWyswietlacza));
                opoznienie_ms(1000);
                czarnePozostalyCzas--; // Dekrementacja czasu czarnych
                if (czarnePozostalyCzas < 1) {
                    aktualnaFaza = 0;
                }
                break;

            case 3: // Odliczanie do rozpocz?cia gry
                odliczanieStartowe();
                aktualnaFaza = 1; // Zaczyna si? od strony bia?ych
                break;

            default:
                break;
        }

        // Resetowanie gry przez przytrzymanie przycisku S6
        if (BUTTON_IsPressed(BUTTON_S6)) {
            aktualnaFaza = 3; 
            bialePozostalyCzas = pozostalyCzas;
            czarnePozostalyCzas = pozostalyCzas;
        }
    }
    return 0;
}