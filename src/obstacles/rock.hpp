#ifndef ROCK_HPP
#define ROCK_HPP

#include "obstacle.hpp"
#include "randomize.hpp"

class Rock : public Obstacle
{
public:
    Rock() : Obstacle({ 17.0f, 24.0f }) {
        int nRock = Randomize::GetRandom(1, 2);

        m_RenderData.viFramePos = { 17 * nRock, 17 };
        m_RenderData.vfRenderSize = { 34.0f, 48.0f };
    }

    void Update(float fElapsedTime) override {
        Obstacle::Update(fElapsedTime);

        if (m_vPosition.y > SCREEN_HEIGHT)
            m_bSelfDestruct = true;
    }

};

#endif // ROCK_HPP
