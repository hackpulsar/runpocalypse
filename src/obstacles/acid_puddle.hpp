#ifndef ACID_PUDDLE_HPP
#define ACID_PUDDLE_HPP

#include "obstacle.hpp"
#include "randomize.hpp"

class AcidPuddle : public Obstacle
{
public:
    AcidPuddle() : Obstacle({ 17.0f, 17.0f }) {
        int nPuddle = Randomize::GetRandom(0, 2);
        m_RenderData.viFramePos = { 17 * nPuddle, 0 };
        m_RenderData.vfRenderSize = { 34.0f, 34.0f };
    }
    
    void Update(float fElapsedTime) override {
        Obstacle::Update(fElapsedTime);

        if (m_vPosition.y > SCREEN_HEIGHT)
            m_bSelfDestruct = true;
    }

};

#endif // ACID_PUDDLE_HP
