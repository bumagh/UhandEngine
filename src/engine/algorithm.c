#include "algorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void generateUUID(char *uuid)
{
    const char *chars = "0123456789abcdef";
    int i;

    srand((unsigned int)time(NULL)); // 使用当前时间初始化随机数生成器

    for (i = 0; i < 36; i++)
    {
        switch (i)
        {
        case 8:
        case 13:
        case 18:
        case 23:
            uuid[i] = '-';
            break;
        case 14:
            uuid[i] = '4'; // 第13个字符必须为 '4'，表示 UUID 版本 4
            break;
        case 19:
            uuid[i] = chars[(rand() % 4) + 8]; // 第17个字符，使用 8、9、a 或 b
            break;
        default:
            uuid[i] = chars[rand() % 16]; // 其余位置使用随机字符
        }
    }
    uuid[36] = '\0'; // 字符串结束符
}