#ifndef ACID_SPIT_HPP
#define ACID_SPIT_HPP

#include "entity.hpp"
#include "animatable.hpp"
#include "collidable.hpp"

class AcidSpit : public Entity, public Animatable, public Collidable
{
public:
    AcidSpit()
        : Entity({ 0.f, 0.f }),
          Animatable("./bin/res/acid_spit_sheet.png", { { 0, 0 }, { 8, 8 }, { 8.f, 8.f } }),
          Collidable({ { 0.f, 0.f }, { 8.f, 8.f } })
    {
        Animatable::LoadAnimation(
            {
                { 0, 0 },
                { 2, 0 },
                3,
                m_RenderData.vfRenderSize,
                m_RenderData.viFrameSize,
                true
            },
            "idle"
        );

        Animatable::AddAnimation("idle");
    }

    void Update(float fElapsedTime) override {
        m_vPosition.y -= ACID_SPIT_SPEED * fElapsedTime;

        if (m_vPosition.y < -8.0f * SCALE)
            m_bSelfDestruct = true;

        Animatable::Update(fElapsedTime);
        Collidable::UpdatePosition(m_vPosition + olc::vf2d(0.f, 16.f));
    }

    void Render(olc::PixelGameEngine& pge) const override {
        Animatable::Render(m_vPosition, pge);
    }

    void AdjustPosition(const int nLane) override {
        if (nLane > 2)
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + MIDDLE_LINE_WIDTH + (LANE_WIDTH / 2 - 2)) * SCALE;
        else
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + (LANE_WIDTH / 2 - 2)) * SCALE;

        m_vPosition.y = SCREEN_HEIGHT - 8 * SCALE;

        Entity::AdjustPosition(nLane);
    }

};

#endif // ACID_SPIT_HPP
