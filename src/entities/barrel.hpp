#ifndef BARREL_HPP
#define BARREL_HPP

#include "entity.hpp"
#include "rendarable.hpp"
#include "collidable.hpp"

#include "settings.hpp"

class Barrel : public Entity, public Renderable, public Collidable
{
public:
    Barrel()
        : Entity({ 0.f, 0.f }),
          Renderable("./bin/res/projectiles.png", { 0, 8 }, { 12, 8 }, { 24.f, 16.f }),
          Collidable({ { 0.f, 0.f }, { 12.f, 8.f } })
    {
        Renderable::SetLayer(2);
    }

    void Update(float fElapsedTime) override {
        m_vPosition.y -= ACID_SPIT_SPEED * fElapsedTime;
        if (m_vPosition.y < -16.0f * SCALE) m_bSelfDestruct = true;

        m_fRotation += BARREL_ROTATION_SPEED * fElapsedTime;
        if (m_fRotation >= 360.0f) m_fRotation = 0.0f;

        Collidable::UpdatePosition(m_vPosition + olc::vf2d(0, 24.f));
    }
    
    void Render(olc::PixelGameEngine& pge) const override {
        //Renderable::Render(m_vPosition, pge);
        pge.DrawPartialRotatedDecal(
            m_vPosition, m_pDecal.get(), m_fRotation, 
            { 6, 4 }, { 0, 8 }, 
            { 12, 8 }, { SCALE, SCALE }
        );
    }

    void AdjustPosition(const int nLane) override {
        if (nLane > 2)
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + MIDDLE_LINE_WIDTH + (LANE_WIDTH / 2 - 2)) * SCALE;
        else
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + (LANE_WIDTH / 2 - 2)) * SCALE;

        m_vPosition.y = SCREEN_HEIGHT - 8 * SCALE;

        Entity::AdjustPosition(nLane);
    }


private:
    float m_fRotation = 0.0f;

};

#endif // BARREL_HPP
