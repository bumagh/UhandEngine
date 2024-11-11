#include "util.h"
#include <math.h>

int pointInRect(float x, float y, SDL_Rect *rect)
{
    if (x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h)
        return 1;
    return 0;
}
float length(float x, float y){
    return SDL_sqrt((x) * (x) + (y) * (y));
}
int pointInCircle(float x, float y, float x2, float y2, float r2){
    if((length(x-x2, y-y2)) < r2)
    return 1;
    return 0;
}

// 计算两个点之间的距离的辅助函数
float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// 检测两个圆是否相交
int circlesIntersect(float x1, float y1, float r1, float x2, float y2, float r2) {
    float dist = distance(x1, y1, x2, y2);  // 计算圆心之间的距离
    if (dist <= (r1 + r2)) {
        return 1;  // 两个圆相交
    }
    return 0;  // 两个圆不相交
}
