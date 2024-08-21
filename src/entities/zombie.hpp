#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "entity.hpp"
#include "animatable.hpp"
#include "collidable.hpp"
#include "settings.hpp"

class Zombie : public Entity, public Animatable, public Collidable
{
public:
    Zombie()
        : Entity({ 0.f, 0.f }),
          Animatable("./bin/res/zombie_sheet.png", { { 0, 0 }, { 16, 32 }, { 25.f, 50.f } }),
          Collidable({ { 0.f, 0.f }, { 16.f, 32.f } })
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
            "idle"
        );

        Renderable::SetLayer(2);
    }

    void Update(float fElapsedTime) override {
        if (!m_bSpawned) {
            m_vPosition.y -= 20.0f * fElapsedTime;

            if (m_vPosition.y <= SCREEN_HEIGHT - 12 * SCALE)
                m_bSpawned = true;

            Animatable::Update(fElapsedTime);

            return;
        }

        m_fLifeTimer += fElapsedTime;
        if (m_fLifeTimer >= m_fLifeSpan)
            m_bLifeEnded = true;

        if (m_bLifeEnded) {
            m_vPosition.y += 20.0f * fElapsedTime;

            if (m_vPosition.y >= SCREEN_HEIGHT)
                m_bSelfDestruct = true;
        }

        if (m_bColided) {
            m_vPosition.y += BACKGROUND_SPEED * fElapsedTime;

            if (m_vPosition.y >= SCREEN_HEIGHT)
                m_bSelfDestruct = true;
        }

        if (m_bEndGame) {
            m_vPosition.y -= BACKGROUND_SPEED * fElapsedTime;

            if (m_vPosition.y <= -50.0f * SCALE)
                m_bSelfDestruct = true;
        }

        Collidable::UpdatePosition(m_vPosition);
    }

    void Render(olc::PixelGameEngine& pge) const override {
        Animatable::Render(m_vPosition, pge);
    }

    void AdjustPosition(const int nLane) override {
        if (nLane > 2)
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + (MIDDLE_LINE_WIDTH - 1) + (LANE_WIDTH / 2 - 7)) * SCALE;
        else
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + (LANE_WIDTH / 2 - 7)) * SCALE;

        m_vPosition.y = SCREEN_HEIGHT + 12 * SCALE;

        Entity::AdjustPosition(nLane);
    }

    virtual void ToggleEndGameAnimation() { m_bEndGame = true; }
    void ToggleColided() { m_bColided = true; }

    bool ReadyToShoot() const { return m_bReadyToShoot; }
    virtual void Shoot() { 
        m_bReadyToShoot = false; 
        m_fShootTimer = 0.0f;
    }

protected:
    float m_fLifeTimer = 0.0f;
    float m_fShootTimer = 0.0f;

    bool m_bSpawned = false;
    bool m_bEndGame = false;
    bool m_bLifeEnded = false;

    bool m_bColided = false;

    bool m_bReadyToShoot = false;

    const float m_fShootDelay = 3.0f;
    const float m_fLifeSpan = 10.0f;

};

#endif // ZOMBIE_HPP
