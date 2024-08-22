#ifndef OBSTAACLE_HPP
#define OBSTAACLE_HPP

#include "entities/entity.hpp"
#include "rendarable.hpp"
#include "collidable.hpp"

#include "randomize.hpp"
#include "settings.hpp"

class Obstacle : public Entity, public Renderable, public Collidable
{
public:
    Obstacle(olc::vf2d vfSpriteSize)
        : Entity({ 0.0f, 0.0f }), 
          Renderable("./bin/res/obstacles_sheet.png", { 0, 0 }, vfSpriteSize, vfSpriteSize, 0),
          Collidable({ { 0.0f, 0.0f }, m_RenderData.vfRenderSize * SCALE })
    { }

    virtual void Update(float fElapsedTime) override {
        if (!m_bEndGame) m_vPosition.y += BACKGROUND_SPEED * fElapsedTime;
        Collidable::UpdatePosition(m_vPosition);
    }

    void Render(olc::PixelGameEngine& pge) const override {
        Renderable::Render(m_vPosition, pge);
    }

    void AdjustPosition(const int nLane) override {
        int nOffset = Randomize::GetRandom(0, 8);

        if (nLane > 2)
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + MIDDLE_LINE_WIDTH + (LANE_WIDTH / 2 - nOffset)) * SCALE;
        else
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + (LANE_WIDTH / 2 - nOffset)) * SCALE;

        m_vPosition.y = -m_RenderData.vfRenderSize.y;
        
        Entity::AdjustPosition(nLane);
    }

};

#endif // OBSTAACLE_HPP
