#include <stdio.h>
#define SIZE 8

int chess[SIZE][SIZE];

typedef struct vec2 vec2;
struct vec2{
    int x, y;
};

// 讀入棋盤內容
void scanChess(void){
    for(int i = 0; i < SIZE; i ++)
        for(int j = 0; j < SIZE; j ++)
            scanf("%1d", &chess[i][j]);
    return;
}

// 印出棋盤內容
void printChess(void){
    for(int i = 0; i < SIZE; i ++){
        for(int j = 0; j < SIZE; j ++)
            if(chess[i][j] == 3) printf("x");
            else printf("%d", chess[i][j]);
        printf("\n");
    }
    return;
}


// 給定座標及尋找方向，回傳可下子座標，無解則回傳(-1, -1)
vec2 solve(vec2 pos1, vec2 dir){
    int color1 = chess[pos1.x][pos1.y];
    if(color1 == 0){
        return pos1;
    }
    vec2 pos2 = pos1;
    int color2;
    int check = 0;
    while(1){
        // 將pos2往dir方向增加
        pos2.x += dir.x;
        pos2.y += dir.y;
        
        // pos2超出邊界
        if(pos2.x < 0 || pos2.x >= SIZE || pos2.y < 0 || pos2.y >= SIZE)
            break;
        color2 = chess[pos2.x][pos2.y];
        // 遇到空格子
        if(color2 == 0){
            // 若pos2與pos1相鄰，則仍是無解
            if(!check)
                break;
            // pos2與pos1不相鄰，回傳pos2
            return pos2;
        }
        // 遇到相同顏色的棋子
        if(color2 == color1)
            break;
        check = 1;
    }
    // 無解，回傳pos1
    return (vec2){-1, -1};
}


int main(){
    // 讀入棋盤內容
    scanChess();

    // 讀入給定座標
    vec2 pos;
    scanf("%d,%d", &pos.x, &pos.y);

    // 印出顏色
    int color = chess[pos.x][pos.y];
    switch (color){
    case 0:
        printf("(%d,%d) 空白，所以不用找周圍能不能下。因為不能下\n", pos.x, pos.y);
        return 0;
    case 1:
        printf("(%d,%d)=1, 黑棋。以下黑棋來說，找出(%d,%d)八方能下黑子的位置是 ", pos.x, pos.y, pos.x, pos.y);
        break;
    case 2:
        printf("(%d,%d)=2, 白棋。以下白棋來說，找出(%d,%d)八方能下白子的位置是 ", pos.x, pos.y, pos.x, pos.y);
        break;
    }
    
    // 八個方向(dir.x, dir.y)都找一次
    int cnt = 0;
    vec2 dir;
    for(dir.x = -1; dir.x <= 1; dir.x ++){
        for(dir.y = -1; dir.y <= 1; dir.y ++){
            // 跳過 dir == (0, 0)
            if(!dir.x && !dir.y)
                continue;

            // 找dir方向
            vec2 ans = solve(pos, dir);
            if(ans.x >= 0){
                // 位置ans可以下，印出結果，並在棋盤上標記該點位置
                if(cnt) printf(", ");
                printf("(%d,%d)", ans.x, ans.y);
                chess[ans.x][ans.y] = 3;
                cnt ++;
            }
        }
    }

    if(cnt == 0){
        // 八個方向都不能下
        printf("無解");
    }
    // 印出棋盤內容
    printf("\n");
    printChess();

    return 0;
}