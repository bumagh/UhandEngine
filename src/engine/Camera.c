#include "Camera.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// 创建摄像机
Camera *Camera_Create(int viewportWidth, int viewportHeight)
{
    Camera *camera = (Camera *)malloc(sizeof(Camera));
    if (!camera)
    {
        printf("Failed to allocate memory for Camera\n");
        return NULL;
    }

    camera->x = 0.0f;
    camera->y = 0.0f;
    camera->zoom = 1.0f;
    camera->rotation = 0.0f;
    camera->viewportWidth = viewportWidth;
    camera->viewportHeight = viewportHeight;
    camera->followTarget = 0;

    return camera;
}

// 销毁摄像机
void Camera_Destroy(Camera *camera)
{
    if (camera)
    {
        free(camera);
    }
}

// 设置位置
void Camera_SetPosition(Camera *camera, float x, float y)
{
    if (camera)
    {
        camera->x = x;
        camera->y = y;
    }
}

// 获取位置
void Camera_GetPosition(Camera *camera, float *x, float *y)
{
    if (camera)
    {
        if (x) *x = camera->x;
        if (y) *y = camera->y;
    }
}

// 移动摄像机
void Camera_Move(Camera *camera, float dx, float dy)
{
    if (camera)
    {
        camera->x += dx;
        camera->y += dy;
    }
}

// 设置缩放
void Camera_SetZoom(Camera *camera, float zoom)
{
    if (camera && zoom > 0.1f)
    {
        camera->zoom = zoom;
    }
}

// 获取缩放
float Camera_GetZoom(Camera *camera)
{
    if (camera)
        return camera->zoom;
    return 1.0f;
}

// 缩放
void Camera_Zoom(Camera *camera, float amount)
{
    if (camera)
    {
        camera->zoom += amount;
        if (camera->zoom < 0.1f)
            camera->zoom = 0.1f;
        if (camera->zoom > 10.0f)
            camera->zoom = 10.0f;
    }
}

// 设置旋转
void Camera_SetRotation(Camera *camera, float rotation)
{
    if (camera)
    {
        camera->rotation = rotation;
    }
}

// 获取旋转
float Camera_GetRotation(Camera *camera)
{
    if (camera)
        return camera->rotation;
    return 0.0f;
}

// 设置视口
void Camera_SetViewport(Camera *camera, int width, int height)
{
    if (camera)
    {
        camera->viewportWidth = width;
        camera->viewportHeight = height;
    }
}

// 世界坐标转屏幕坐标
void Camera_WorldToScreen(Camera *camera, float worldX, float worldY, int *screenX, int *screenY)
{
    if (!camera)
        return;

    // 相对于摄像机位置
    float relX = worldX - camera->x;
    float relY = worldY - camera->y;

    // 应用缩放
    relX *= camera->zoom;
    relY *= camera->zoom;

    // 应用旋转
    if (camera->rotation != 0.0f)
    {
        float cosR = cosf(camera->rotation);
        float sinR = sinf(camera->rotation);
        float rotatedX = relX * cosR - relY * sinR;
        float rotatedY = relX * sinR + relY * cosR;
        relX = rotatedX;
        relY = rotatedY;
    }

    // 加上视口中心偏移
    if (screenX) *screenX = (int)(relX + camera->viewportWidth / 2);
    if (screenY) *screenY = (int)(relY + camera->viewportHeight / 2);
}

// 屏幕坐标转世界坐标
void Camera_ScreenToWorld(Camera *camera, int screenX, int screenY, float *worldX, float *worldY)
{
    if (!camera)
        return;

    // 减去视口中心偏移
    float relX = screenX - camera->viewportWidth / 2;
    float relY = screenY - camera->viewportHeight / 2;

    // 应用逆旋转
    if (camera->rotation != 0.0f)
    {
        float cosR = cosf(-camera->rotation);
        float sinR = sinf(-camera->rotation);
        float rotatedX = relX * cosR - relY * sinR;
        float rotatedY = relX * sinR + relY * cosR;
        relX = rotatedX;
        relY = rotatedY;
    }

    // 应用逆缩放
    relX /= camera->zoom;
    relY /= camera->zoom;

    // 加上摄像机位置
    if (worldX) *worldX = relX + camera->x;
    if (worldY) *worldY = relY + camera->y;
}

// 获取视口矩形
SDL_Rect Camera_GetViewportRect(Camera *camera)
{
    SDL_Rect rect = {0, 0, 0, 0};
    if (camera)
    {
        rect.w = camera->viewportWidth;
        rect.h = camera->viewportHeight;
    }
    return rect;
}

// 应用摄像机变换到渲染器
void Camera_Apply(Camera *camera, SDL_Renderer *renderer)
{
    if (!camera || !renderer)
        return;

    // SDL 不直接支持摄像机变换，需要手动计算
    // 这里只提供接口，实际应用需要在渲染时使用坐标转换
}
