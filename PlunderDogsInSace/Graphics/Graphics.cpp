#include "Graphics.h"

Graphics::Graphics() : m_screenWidth(1280.f), m_screenHeight(720.f)
{
}

Graphics::Graphics(float ScreenWidth, float ScreenHeight) : m_screenWidth(ScreenWidth), m_screenHeight(m_screenHeight)
{
}
