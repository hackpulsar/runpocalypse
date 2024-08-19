#ifndef CAR_HPP
#define CAR_HPP

#include "obstacle.hpp"
#include "randomize.hpp"

class Car : public Obstacle
{
public:
    Car() : Obstacle({ 20.0f, 41.0f }) {
        int nCar = Randomize::GetRandom(0, 3);

        m_RenderData.viFramePos = { 20 * nCar, 41 };
        m_RenderData.vfRenderSize = { 40.0f, 82.0f };
    }

    void Update(float fElapsedTime) override {
        Obstacle::Update(fElapsedTime);

        if (m_vPosition.y > SCREEN_HEIGHT)
            m_bSelfDestruct = true;
    }
};

#endif // CAR_HPP
