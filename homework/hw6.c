#include <stdio.h>
#include <string.h>
#define SIZE 8

typedef struct vec2 vec2;
struct vec2{
    int x, y;
};

void resetChess(int chess[SIZE][SIZE]){
    memset(chess, 0, sizeof(int) * SIZE * SIZE);
    chess[3][3] = 1;
    chess[3][4] = 2;
    chess[4][3] = 2;
    chess[4][4] = 1;
    return;
}

// 印出棋盤內容
void printChess(int chess[SIZE][SIZE]){
    for(int i = 0; i < SIZE; i ++){
        for(int j = 0; j < SIZE; j ++)
            printf("%d ", chess[i][j]);
        printf("\n");
    }
    return;
}

// 印出所有可下的座標
void printNext(int next[SIZE][SIZE]){
    int first = 1;
    for(int i = 0; i < SIZE; i ++){
        for(int j = 0; j < SIZE; j ++){
            if(next[i][j]){
                if(!first) printf(", ");
                printf("(%d, %d)", i, j);
                first = 0;
            }
        }
    }
    return;
}

// 給定座標及尋找方向，回傳可下子座標與給定座標間的距離，無解則回傳0
int newStep(int chess[SIZE][SIZE], vec2 pos1, vec2 dir){
    
    int color1 = chess[pos1.x][pos1.y];
    if(color1 == 0){
        return 0;
    }
    // 從pos1出發往外找
    vec2 pos2 = pos1;
    int color2;
    int len = 0;
    while(1){
        // 將pos2往dir方向增加
        pos2.x += dir.x;
        pos2.y += dir.y;
        len ++;
        
        // pos2超出邊界
        if(pos2.x < 0 || pos2.x >= SIZE || pos2.y < 0 || pos2.y >= SIZE)
            break;
        color2 = chess[pos2.x][pos2.y];
        // 遇到空格子
        if(color2 == 0){
            // 若pos2與pos1相鄰，則仍是無解
            if(len <= 1)
                break;
            // pos2與pos1不相鄰，回傳pos2
            return len;
        }
        // 遇到相同顏色的棋子
        if(color2 == color1)
            break;
    }
    // 無解，回傳0
    return 0;
}

// 給定一點pos，將所有因為點pos的存在而可下子的座標標記在next[][]中
void checkNewStep(int chess[SIZE][SIZE], int next[SIZE][SIZE], vec2 pos) 
{
    vec2 dir;
    for(dir.x = -1; dir.x <= 1; dir.x ++){
        for(dir.y = -1; dir.y <= 1; dir.y ++){
            // 跳過 dir == (0, 0)
            if(!dir.x && !dir.y)
                continue;

            // 找dir方向
            int res = newStep(chess, pos, dir);
            if(res > 0){
                // 位置ans可以下
                vec2 ans = {pos.x + dir.x * res, pos.y + dir.y * res};
                // 在棋盤上標記該點位置
                next[ans.x][ans.y] = 1;
            }
        }
    }
    return;
}

// 給定一個顏色color，將該顏色所有可下子的座標標記在next[][]中
void checkNewStepByColor(int chess[SIZE][SIZE], int next[SIZE][SIZE], int color) 
{
    vec2 pos;
    // 檢查棋盤上所有顏色為color的棋子
    for(pos.x = 0; pos.x < SIZE; pos.x ++){
        for(pos.y = 0; pos.y < SIZE; pos.y ++){
            int c = chess[pos.x][pos.y];
            if(c == color)
                checkNewStep(chess, next, pos);
        }
    }
    return;
}

// 在位置pos下顏色color的棋，將dir方向的結果標記在change中
void checkChangeDir(int chess[SIZE][SIZE], int change[SIZE][SIZE], vec2 pos, int color, vec2 dir){
    vec2 pos2 = pos;
    while(1){
        pos2.x += dir.x;
        pos2.y += dir.y;
        // pos2超出邊界
        if(pos2.x < 0 || pos2.x >= SIZE || pos2.y < 0 || pos2.y >= SIZE)
            return;
        int color2 = chess[pos2.x][pos2.y];
        if(color2 == 0)
            return;
        if(color2 == color)
            break;
    }
    // 還沒return表示該路線可翻
    pos2 = pos;
    while(1){
        pos2.x += dir.x;
        pos2.y += dir.y;
        int color2 = chess[pos2.x][pos2.y];
        if(color2 == color)
            break;
        // 記錄
        change[pos2.x][pos2.y] = 1;
    }
    return;
}

// 將在位置pos下顏色color的棋，會被翻到的所有棋子座標標記在change中
void checkChange(int chess[SIZE][SIZE], int change[SIZE][SIZE], vec2 pos, int color)
{
    vec2 dir;
    for(dir.x = -1; dir.x <= 1; dir.x ++){
        for(dir.y = -1; dir.y <= 1; dir.y ++){
            // 跳過 dir == (0, 0)
            if(!dir.x && !dir.y)
                continue;

            // 翻dir方向
            checkChangeDir(chess, change, pos, color, dir);
        }
    }
    return;
}

// 計算array中有幾個非零的數字
int countArray(int array[SIZE][SIZE]){
    int cnt = 0;
    vec2 pos;
    for(pos.x = 0; pos.x < SIZE; pos.x ++){
        for(pos.y = 0; pos.y < SIZE; pos.y ++){
            if(array[pos.x][pos.y])
                cnt ++;
        }
    }
    return cnt;
}

// 從所有可走的位置中，選出一個能翻轉最多棋子的位置
vec2 chooseNextPos(int chess[SIZE][SIZE], int next[SIZE][SIZE], int color){
    vec2 ans = {-1, -1};
    int max = 0;
    vec2 pos;
    for(pos.x = 0; pos.x < SIZE; pos.x ++){
        for(pos.y = 0; pos.y < SIZE; pos.y ++){
            if(next[pos.x][pos.y]){
                int change[SIZE][SIZE] = {};
                checkChange(chess, change, pos, color);
                int cnt = countArray(change);
                if(cnt > max){
                    // 更新最佳解
                    max = cnt;
                    ans = pos;
                }
            }
        }
    }
    return ans;
}

// 翻棋子
void flipChess(int chess[SIZE][SIZE], int change[SIZE][SIZE]){
    vec2 pos;
    // 走訪棋盤上所有棋子
    for(pos.x = 0; pos.x < SIZE; pos.x ++){
        for(pos.y = 0; pos.y < SIZE; pos.y ++){
            if(change[pos.x][pos.y])
                // 翻棋子
                chess[pos.x][pos.y] ^= 3;
        }
    }
    return;
}

int main(){
    // 遊戲初始化
    int chess[SIZE][SIZE];
    resetChess(chess);
    int current_color;
    
    while(1){
        // 換玩家
        current_color = 1;
        {
            printf("1 黑棋你下\n");
            printChess(chess);

            // 尋找所有可下子的位置
            int next[SIZE][SIZE] = {0};
            checkNewStepByColor(chess, next, current_color);
            // 若沒有地方可下子，則離開迴圈
            if(countArray(next) == 0)
                break;
            printNext(next);

            // 決定一個要下子的位置
            vec2 pos;
            // 要求玩家輸入合法座標到pos中，若該座標不合法則重新要求
            printf("\n請下合法位子 x y\n");
            scanf("%d %d", &pos.x, &pos.y);
            while(pos.x < 0 || pos.x >= SIZE || pos.y < 0 || pos.y >= SIZE || !next[pos.x][pos.y]){
                printf("請下合法位子 x y\n");
                scanf("%d %d", &pos.x, &pos.y);
            }

            // 在pos處下子，並更新棋盤
            chess[pos.x][pos.y] = current_color;
            int change[SIZE][SIZE] = {0};
            checkChange(chess, change, pos, current_color);
            flipChess(chess, change);
        }
        // 換電腦
        current_color = 2;
        {
            printf("2 電腦白棋下\n");

            // 尋找所有可下子的位置
            int next[SIZE][SIZE] = {0};
            checkNewStepByColor(chess, next, current_color);
            // 若沒有地方可下子，則離開迴圈
            if(countArray(next) == 0)
                break;

            // 決定出一個要下子的位置
            vec2 pos = chooseNextPos(chess, next, current_color);

            // 在pos處下子，並更新棋盤
            chess[pos.x][pos.y] = current_color;
            int change[SIZE][SIZE] = {0};
            checkChange(chess, change, pos, current_color);
            flipChess(chess, change);
            printf("電腦下 %d, %d\n", pos.x, pos.y);
        }
    }

    int cnt[3] = {0};
    vec2 pos;
    int color;
    for(pos.x = 0; pos.x < SIZE; pos.x ++){
        for(pos.y = 0; pos.y < SIZE; pos.y ++){
            color = chess[pos.x][pos.y];
            cnt[color] ++;
        }
    }
    printf("棋局結束\n");
    if(cnt[1] > cnt[2])
        printf("白方勝 %d:%d", cnt[1], cnt[2]);
    else if(cnt[2] > cnt[1])
        printf("黑方勝 %d:%d", cnt[2], cnt[1]);
    else
        printf("平局 %d:%d", cnt[1], cnt[2]);
    

        
    return 0;
}
