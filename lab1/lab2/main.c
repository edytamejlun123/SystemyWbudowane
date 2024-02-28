#include <stdio.h>
#include <stdlib.h>

#define BIT_VALUE(value, noBit) (value >> noBit) & 1

int main()
{
    int seed = 1;
    int val  = BIT_VALUE(seed,0) ;
    printf("bit_0: %d \n", val);

    for (int j=1; j<70; j++){
        for( int i=1; i<6; i++){
            if(BIT_VALUE(51, i) == 1){
                val = val ^ BIT_VALUE(seed,i);
            }
        }

        seed = (seed>>1) + (val*32);

        for(int i = 5; i>=0; i--){
            printf("%d", BIT_VALUE(seed, i));
        }

        printf("\n");
        j++;
    }
    return 0;
}
