#ifndef ROCKET_HPP
#define ROCKET_HPP

#include "entity.hpp"
#include "animatable.hpp"
#include "collidable.hpp"

class Rocket : public Entity, public Animatable, public Collidable
{
public:
    Rocket(olc::vi2d vPosition = { 0, 0 }) 
        : Entity(vPosition), 
          Animatable("./bin/res/rocket_sheet.png", { { 0, 0 }, { 8, 40 }, { 12.f, 60.f } }),
          Collidable({ vPosition, { 8.f, 8.f } })
    {
        Animatable::LoadAnimation(
            {
                { 0, 0 },
                { 1, 0 },
                2,
                m_RenderData.vfRenderSize,
                m_RenderData.viFrameSize,
                true
            },
            "fly"
        );

        Animatable::AddAnimation("fly");

        Renderable::SetLayer(2);
    }

    void Update(float fElapsedTime) override {
        m_vPosition.y -= ROCKET_SPEED * fElapsedTime;

        if (m_vPosition.y < -60)
            m_bSelfDestruct = true;

        Animatable::Update(fElapsedTime);
        Collidable::UpdatePosition(m_vPosition + olc::vf2d(0.f, 32.f - 8.f));
    }
    
    void Render(olc::PixelGameEngine& pge) const override {
        Animatable::Render(m_vPosition, pge);
    }

    void AdjustPosition(const int nLane) override {
        if (nLane > 2)
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + MIDDLE_LINE_WIDTH + (LANE_WIDTH / 2 - 4)) * SCALE;
        else
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + (LANE_WIDTH / 2 - 4)) * SCALE;

        m_vPosition.y = SCREEN_HEIGHT + 60 * SCALE;

        Entity::AdjustPosition(nLane);
    }

};

#endif // ROCKET_HPP
