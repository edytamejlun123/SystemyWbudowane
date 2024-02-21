#include <stdio.h>
#include <stdlib.h>

#define BIT_VALUE(value, noBit) (value >> noBit) & 1

unsigned int counter = 0;
void delay(){
    int a,b;
    for(a=0; a<=1474836477; a++);{
        for(b=0; b<=512; b++);{

        }
    }
}

int main()
{
    printf("\n\n\n");
    //Zadanie 1.
    while(1){
        printf("\r");
        for(int i = 7; i>=0; i--){
            printf("%d", BIT_VALUE(counter, i));
        }
        counter ++;
        delay();
    }

    //Zadanie 2. kod greya (aby sprawdzi� kod greya, najlepiej skomentowa� zad 1.)
    int IntToGray(unsigned char input){
        return (input>>1)^input;
    }
    while(1){
        printf("\r");
        for(int i = 7; i>=0; i--){
            printf("%d", BIT_VALUE(counter, i));
        }
        printf("    ");
        for(int i=7; i>=0; i--){
            printf("%d", BIT_VALUE(IntToGray(counter), i));
        }
        counter++;
        delay();
    }


    return 0;

}
