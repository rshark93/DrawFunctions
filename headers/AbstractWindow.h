#pragma once

//#include <SDL_events.h>
#include <memory>
#include <vector>

typedef struct vec2 {
    int x;
    int y;
} vec2;

typedef struct vec4 {
    int a;
    int r;
    int g;
    int b;
} vec4;

class CAbstractWindow
{
//public:
//    CAbstractWindow();
//    virtual ~CAbstractWindow();
//
//    void ShowFixedSize(vec2 const& size);
//    void DoGameLoop();
//
//protected:
//    void SetBackgroundColor(vec4 const& color);
//
//    virtual void OnWindowEvent(const SDL_Event &event) = 0;
//    virtual void OnUpdateWindow(float deltaSeconds) = 0;
//    virtual void OnDrawWindow() = 0;
//
//private:
//    class Impl;
//    std::unique_ptr<Impl> m_pImpl;
};