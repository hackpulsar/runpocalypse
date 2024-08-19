#ifndef SMALL_TREE_HPP
#define SMALL_TREE_HPP

#include "obstacle.hpp"

class SmallTree : public Obstacle
{
public:
    SmallTree() : Obstacle({ 17.0f, 24.0f }) {
        m_RenderData.viFramePos = { 17 * 3 + 20, 17 };
        m_RenderData.vfRenderSize = { 34.0f, 48.0f };
    }

    void Update(float fElapsedTime) override {
        Obstacle::Update(fElapsedTime);

        if (m_vPosition.y > SCREEN_HEIGHT)
            m_bSelfDestruct = true;
    }
};

#endif // SMALL_TREE_HPP
