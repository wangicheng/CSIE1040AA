#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define STACK_SIZE 10000
#define MAP_SIZE 10

typedef struct {
    int x, y;
} Point;

bool isEqual(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

typedef struct {
    Point stack[STACK_SIZE];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, Point p) {
    s->stack[++s->top] = p;
}

Point pop(Stack *s) {
    return s->stack[s->top--];
}

Point top(Stack *s) {
    return s->stack[s->top];
}

typedef struct {
    bool map[MAP_SIZE][MAP_SIZE];
} Map;

// void initMap(Map *m) {
//     srand(time(NULL));
//     int x, y;
//     for (x = 0; x < MAP_SIZE; x++) {
//         for (y = 0; y < MAP_SIZE; y++) {
//             m->map[x][y] = (rand() & 1) == 0;
//         }
//     }
// }

void initMap(Map *m) {
    int x, y, val;
    for (x = 0; x < MAP_SIZE; x++) {
        for (y = 0; y < MAP_SIZE; y++) {
            scanf("%d", &val);
            m->map[x][y] = val == 1;
        }
    }
}

void printMap(Map *m) {
    int x, y;
    for (x = 0; x < MAP_SIZE; x ++) {
        for (y = 0; y < MAP_SIZE; y ++) {
            printf("%d ", m->map[x][y]);
        }
        printf("\n");
    }
}

typedef struct {
    Point path[MAP_SIZE * MAP_SIZE];
    int length;
} Path;

void printPath(Path *p) {
    if(p->length <= 0) {
        printf("No path found\n");
        return;
    }
    int i;
    for (i = 0; i < p->length; i++) {
        printf("%d %d\n", p->path[i].x, p->path[i].y);
    }
}

bool isValid(Point p) {
    return p.x >= 0 && p.x < MAP_SIZE && p.y >= 0 && p.y < MAP_SIZE;
}

bool findPath(Map *m, Point start, Point end, Path *p) {
    Stack stack;
    initStack(&stack);
    Point point;
    int steps[MAP_SIZE][MAP_SIZE] = {};
    
    int d, x, y;
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, 1, 0, -1};
    for(x = 0; x < MAP_SIZE; x ++) {
        for(y = 0; y < MAP_SIZE; y ++) {
            steps[x][y] = -1;
        }
    }

    push(&stack, start);
    steps[start.x][start.y] = 0;
    p->path[0] = start;


    while(!isEmpty(&stack)) {
        point = pop(&stack);
        p->path[steps[point.x][point.y]] = point;
        if(isEqual(point, end)) {
            p->length = steps[point.x][point.y] + 1;
            return true;
        }
        for(d = 0; d < 4; d ++) {
            x = point.x + dx[d];
            y = point.y + dy[d];
            if(isValid((Point){x, y}) && m->map[x][y] == 0 && steps[x][y] == -1) {
                steps[x][y] = steps[point.x][point.y] + 1;
                push(&stack, (Point){x, y});
            }
        }
    }
    p->length = -1;
    return false;
}

int main() {
    Point p1 = {0, 0};
    Point p2 = {MAP_SIZE - 1, MAP_SIZE - 1};
    Map map;
    initMap(&map);
    printMap(&map);
    Path path;
    findPath(&map, p1, p2, &path);
    printPath(&path);

    return 0;
}
