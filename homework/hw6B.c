#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define SIZE 8
#define SPACE 0
#define BLACK 1
#define WHITE 2

typedef struct vec2 vec2;
struct vec2{ int x, y; };

// 回傳座標 pos 是否在棋盤內
bool inChess(const vec2 pos){
    return pos.x>=0 && pos.x<SIZE && pos.y>=0 && pos.y<SIZE;
}

// 初始化棋盤
void resetChess(int chess[SIZE][SIZE]){
    memset(chess, 0, sizeof(int) * SIZE * SIZE);
    chess[3][3] = WHITE;
    chess[3][4] = BLACK;
    chess[4][3] = BLACK;
    chess[4][4] = WHITE;
    return;
}

// 印出棋盤
void printChess(const int chess[SIZE][SIZE]){
    int x, y;
    for(int x = 0; x < SIZE; x ++){
        for(int y = 0; y < SIZE; y ++)
            printf("%d ", chess[x][y]);
        printf("\n");
    }
    return;
}

// 印出所有在 choices 中標記為可下子的位置
void printChoices(const int choices[SIZE][SIZE]){
    int x, y;
    bool first = true;
    for(int x = 0; x < SIZE; x ++)
        for(int y = 0; y < SIZE; y ++)
            if(choices[x][y]){
                if(first){
                    printf("(%d, %d)", x, y);
                    first = false;
                }
                else printf(", (%d, %d)", x, y);
            }
    printf("\n");
    return;    
}

// 回傳 choices 裡有無位置可下子
bool haveChoices(const int choices[SIZE][SIZE]){
    for(int x = 0; x < SIZE; x ++)
        for(int y = 0; y < SIZE; y ++)
            if(choices[x][y]) return true;
    return false;
}

// 回傳在 pos 下 color 顏色的棋後可在 dir 方向上翻的棋子數量
int countChangesByPosAndDir(const int chess[SIZE][SIZE], const vec2 pos, const vec2 dir, const int color){
    vec2 checking_pos = {
        pos.x + dir.x,
        pos.y + dir.y
    };
    int cnt = 0; // 記錄可翻子的數量
    while(inChess(checking_pos)){
        if(chess[checking_pos.x][checking_pos.y] == SPACE)
            // 遇到空白表示不能下
            return 0;
        
        if(chess[checking_pos.x][checking_pos.y] == color){
            // 回傳可翻子的數量
            return cnt;
        }
        checking_pos.x += dir.x;
        checking_pos.y += dir.y;
        cnt ++;
    }
    return 0;
}

// 回傳在 pos 下 color 顏色的棋後可翻的棋子數量
int countChangesByPos(const int chess[SIZE][SIZE], const vec2 pos, const int color){
    int cnt = 0; // 記錄可翻子的數量
    // 八個方向個別判斷
    vec2 dir;
    for(dir.x = -1; dir.x <= 1; dir.x ++){
        for(dir.y = -1; dir.y <= 1; dir.y ++){
            // 跳過 (0, 0) 方向
            if(!dir.x && !dir.y) continue;
            // 判斷 dir 方向
            cnt += countChangesByPosAndDir(chess, pos, dir, color);
        }
    }
    return cnt;
}

// 計算把 color 顏色的子下在每個位置時能翻的棋子數量，並記錄在 choices 中
void countChangesByColor(const int chess[SIZE][SIZE], int choices[SIZE][SIZE], const int color){
    vec2 pos;
    for(pos.x = 0; pos.x < SIZE; pos.x ++){
        for(pos.y = 0; pos.y < SIZE; pos.y ++)
            if(chess[pos.x][pos.y] == SPACE)
                choices[pos.x][pos.y] = countChangesByPos(chess, pos, color);
    }
    return;
}

// 讓玩家輸入一個可下子的座標，該座標無法下子則重新輸入
vec2 chooseByPlayer(int choices[SIZE][SIZE], int color){
    vec2 pos;
    while(true){
        printf("請下合法位置 x y\n");
        scanf("%d %d", &pos.x, &pos.y);
        if(!inChess(pos))
            // 輸入棋盤以外的座標
            continue;
        if(choices[pos.x][pos.y])
            // 輸入的座標可以下子
            break;
    }
    return pos;   
}

// 讓電腦選擇一個可下子的座標
vec2 chooseByComputer(const int choices[SIZE][SIZE], int color){
    vec2 pos, best_pos = {0, 0};
    // 走訪 choices 的每個位子，找到能翻最多棋子的座標
    for(pos.x = 0; pos.x < SIZE; pos.x ++)
        for(pos.y = 0; pos.y < SIZE; pos.y ++)
            // 若 pos 能翻的棋比 best_pos 多，則更新 best_pos
            if(choices[best_pos.x][best_pos.y] < choices[pos.x][pos.y])
                best_pos = pos;
    return best_pos;
}

// 從 pos 位置往 dir 方向翻棋子
void flipChessByDir(int chess[SIZE][SIZE], const vec2 pos, const vec2 dir){
    int color = chess[pos.x][pos.y];
    vec2 flipping_pos = {
        pos.x + dir.x,
        pos.y + dir.y
    };
    // 取得該方向可翻棋的數量
    int times = countChangesByPosAndDir(chess, pos, dir, color);
    while(times--){
        // 翻棋子
        chess[flipping_pos.x][flipping_pos.y] = color;
        flipping_pos.x += dir.x;
        flipping_pos.y += dir.y;
    }
    return;
}

// 從 pos 位置往八個方向翻棋子
void flipChess(int chess[SIZE][SIZE], const vec2 pos){
    vec2 dir;
    // 八個方向個別翻棋子
    for(dir.x = -1; dir.x <= 1; dir.x ++){
        for(dir.y = -1; dir.y <= 1; dir.y ++){
            // 跳過 (0, 0) 方向
            if(!dir.x && !dir.y) continue;
            // 往 dir 方向翻棋子
            flipChessByDir(chess, pos, dir);
        }
    }
    return;
}

// 換玩家方，若遊戲結束則回傳 true
bool playerTurn(int chess[SIZE][SIZE]){
    printf("%d 黑棋你下\n", BLACK);
    // 印出棋盤
    printChess(chess);
    // 找出所有可下子的位置
    int choices[SIZE][SIZE] = {0};
    countChangesByColor(chess, choices, BLACK);
    // 無法下子，遊戲結束
    if(!haveChoices(choices))
        return true;
    // 叫使用者選一個要下子的位置
    printChoices(choices);
    vec2 pos = chooseByPlayer(choices, BLACK);
    // 在 pos 位置下棋子
    chess[pos.x][pos.y] = BLACK;
    flipChess(chess, pos);
    return false;
}

// 換電腦方，若遊戲結束則回傳 true
bool computerTurn(int chess[SIZE][SIZE]){
    printf("%d 白棋電腦下\n", WHITE);
    // 找出所有可下子的位置
    int choices[SIZE][SIZE] = {0};
    countChangesByColor(chess, choices, WHITE);
    // 無法下子，遊戲結束
    if(!haveChoices(choices))
        return true;
    // 讓電腦選一個要下子的位置
    vec2 pos = chooseByComputer(choices, WHITE);
    // 在 pos 位置下棋子
    chess[pos.x][pos.y] = WHITE;
    flipChess(chess, pos);
    printf("電腦下 %d, %d\n", pos.x, pos.y);
    return false;
    
}

// 開始遊戲
void startMatch(int chess[SIZE][SIZE]){
    bool isOver = false;
    while(true){
        isOver = playerTurn(chess);
        if(isOver) break;
        isOver = computerTurn(chess);
        if(isOver) break;
    }
    return;
}

// 印出遊戲結果
void printMatchResult(int chess[SIZE][SIZE]){
    // 印出棋盤
    printChess(chess);
    printf("棋局結束\n");
    // 將空白、黑棋、白棋的總數存在 sum 中
    int sum[3] = {0};
    for(int x = 0; x < SIZE; x ++)
        for(int y = 0; y < SIZE; y ++)
            sum[chess[x][y]] ++;
    // 判斷勝負
    if(sum[BLACK] > sum[WHITE])
        printf("黑方勝 %d:%d", sum[BLACK], sum[WHITE]);
    else if(sum[WHITE] > sum[BLACK])
        printf("白方勝 %d:%d", sum[WHITE], sum[BLACK]);
    else
        printf("平局 %d:%d", sum[BLACK], sum[WHITE]);
    return;
}

int main(void) {
    // 製作棋盤
    int chess[SIZE][SIZE];
    resetChess(chess);
    // 開始遊戲
    startMatch(chess);
    // 印出遊戲結果
    printMatchResult(chess);
    return 0;
}
