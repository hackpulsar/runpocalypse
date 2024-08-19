#ifndef BUSH_HPP
#define BUSH_HPP

#include "obstacle.hpp"

class Bush : public Obstacle
{
public:
    Bush() : Obstacle({ 17.0f, 17.0f }) {
        m_RenderData.viFramePos = { 17 * 3 + 20, 0 };
        m_RenderData.vfRenderSize = { 34.0f, 34.0f };
    }

    void Update(float fElapsedTime) override {
        Obstacle::Update(fElapsedTime);

        if (m_vPosition.y > SCREEN_HEIGHT)
            m_bSelfDestruct = true;
    }
};

#endif // BUSH_HPP
