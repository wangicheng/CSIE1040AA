#include <stdio.h>

int main(){
    for(int i = 0; i < 81; i++){
        int x = i / 27 * 3 + (i % 3) + 1;
        int y = (i % 27) / 3 + 1;
        printf("%d x %d = %2d", x, y, x * y);
        if(i % 3 == 2) printf("\n");
        else printf("  ");
        if(x % 3 == 0 && y == 9) printf("\n");
    }
    return 0;
}