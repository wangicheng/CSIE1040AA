#include <stdio.h>

int main(){
    for(int i = 0; i < 81; i++){
        // 計算當前的 x 與 y
        int x = i / 27 * 3 + (i % 3) + 1;
        int y = (i % 27) / 3 + 1;
        // 印出等式
        printf("%d x %d = %2d  ", x, y, x * y);
        // 每印 3 次要換一行
        if(i % 3 == 2) printf("\n");
        // 每印 27 次要多換一行
        if(i % 27 == 26) printf("\n");
    }
    return 0;
}
