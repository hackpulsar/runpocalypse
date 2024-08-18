#ifndef ROCKET_HPP
#define ROCKET_HPP

#include "entity.hpp"
#include "animatable.hpp"
#include "collidable.hpp"

class Rocket : public Entity, Animatable, public Collidable
{
public:
    Rocket(olc::vi2d vPosition = { 0, 0 }) 
        : Entity(vPosition), Collidable({ vPosition, { 8.f, 60.f } })
    {
        m_pSpritesheet = std::make_unique<olc::Sprite>("./bin/res/rocket_sheet.png");
        m_pDecal = std::make_unique<olc::Decal>(m_pSpritesheet.get());

        Animatable::LoadAnimation(
            {
                { 0, 0 },
                { 1, 0 },
                2,
                { 12.f, 60.f },
                { 8, 40 },
                true
            },
            "fly"
        );

        Animatable::AddAnimation("fly");
    }

    void Update(float fElapsedTime) override {
        m_vPosition.y -= ROCKET_SPEED * fElapsedTime;

        if (m_vPosition.y < -60)
            m_bSelfDestruct = true;

        Animatable::Update(fElapsedTime);
        Collidable::UpdatePosition(m_vPosition);
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

        Collidable::UpdatePosition(m_vPosition);
        Entity::AdjustPosition(nLane);
    }

};

#endif // ROCKET_HPP
