#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

// 摄像机结构
typedef struct Camera {
    float x, y;           // 摄像机位置
    float zoom;           // 缩放
    float rotation;       // 旋转（弧度）
    int viewportWidth;    // 视口宽度
    int viewportHeight;   // 视口高度
    int followTarget;     // 是否跟随目标
} Camera;

// 函数声明
Camera *Camera_Create(int viewportWidth, int viewportHeight);
void Camera_Destroy(Camera *camera);

// 位置控制
void Camera_SetPosition(Camera *camera, float x, float y);
void Camera_GetPosition(Camera *camera, float *x, float *y);
void Camera_Move(Camera *camera, float dx, float dy);

// 缩放控制
void Camera_SetZoom(Camera *camera, float zoom);
float Camera_GetZoom(Camera *camera);
void Camera_Zoom(Camera *camera, float amount);

// 旋转控制
void Camera_SetRotation(Camera *camera, float rotation);
float Camera_GetRotation(Camera *camera);

// 视口控制
void Camera_SetViewport(Camera *camera, int width, int height);

// 世界坐标和屏幕坐标转换
void Camera_WorldToScreen(Camera *camera, float worldX, float worldY, int *screenX, int *screenY);
void Camera_ScreenToWorld(Camera *camera, int screenX, int screenY, float *worldX, float *worldY);

// 获取视口矩形（用于裁剪）
SDL_Rect Camera_GetViewportRect(Camera *camera);

// 应用摄像机变换到渲染器
void Camera_Apply(Camera *camera, SDL_Renderer *renderer);

#endif // CAMERA_H
