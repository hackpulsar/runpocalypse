#ifndef ACID_BARREL_HPP
#define ACID_BARREL_HPP

#include "obstacle.hpp"

class AcidBarrel : public Obstacle
{
public:
    AcidBarrel() : Obstacle({ 17.0f, 24.0f }) {
        m_RenderData.viFramePos = { 0, 17 };
        m_RenderData.vfRenderSize = { 34.0f, 48.0f };
    }

    void Update(float fElapsedTime) override {
        Obstacle::Update(fElapsedTime);

        if (m_vPosition.y > SCREEN_HEIGHT)
            m_bSelfDestruct = true;
    }

};

#endif // ACID_BARREL_HPP
