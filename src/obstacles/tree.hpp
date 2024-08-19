#ifndef TREE_HPP
#define TREE_HPP

#include "obstacle.hpp"

class Tree : public Obstacle
{
public:
    Tree() : Obstacle({ 20.0f, 41.0f }) {
        m_RenderData.viFramePos = { 17 * 3, 0 };
        m_RenderData.vfRenderSize = { 40.0f, 82.0f };
    }

    void Update(float fElapsedTime) override {
        Obstacle::Update(fElapsedTime);

        if (m_vPosition.y > SCREEN_HEIGHT)
            m_bSelfDestruct = true;
    }

};

#endif // TREE_HPP
