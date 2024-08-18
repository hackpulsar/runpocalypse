#ifndef SPIT_ZOMBIE_HPP
#define SPIT_ZOMBIE_HPP

#include "entity.hpp"
#include "animatable.hpp"

class SpitZombie : public Entity, Animatable
{
public:
    SpitZombie()
        : Entity({ 0.0f, 0.0f })
    {
        m_pSpritesheet = std::make_unique<olc::Sprite>("./bin/res/zombie_sheet.png");
        m_pDecal = std::make_unique<olc::Decal>(m_pSpritesheet.get());

        Animatable::LoadAnimation(
            {
                { 0, 0 },
                { 1, 0 },
                2,
                { 25.f, 50.f },
                { 16, 32 },
                true
            },
            "idle"
        );

        Animatable::LoadAnimation(
            {
                { 0, 1 },
                { 7, 1 },
                8,
                { 25.f, 50.f },
                { 16, 32 },
                false
            },
            "spit"
        );

        Animatable::AddAnimation("idle");
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
        m_fSpitTimer += fElapsedTime;

        if (m_bPrepareToSpit)
            m_fSpitReleaseTimer += fElapsedTime;

        if (m_fLifeTimer >= m_fLifeSpan)
            m_bSelfDestruct = true;

        if (m_fSpitTimer >= m_fSpitFrequency) {
            m_fSpitTimer = 0.0f;
            Animatable::AddAnimation("spit");
            m_bPrepareToSpit = true;
        }
        
        if (m_fSpitReleaseTimer >= m_fSpitDelay) {
            m_bPrepareToSpit = false;
            m_bReadyToSpit = true;
        }

        Animatable::Update(fElapsedTime);
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

    bool ReadyToSpit() const { return m_bReadyToSpit; }
    void Spit() { 
        m_fSpitReleaseTimer = 0.0f;
        m_bReadyToSpit = false; 
    };

private:
    // DEBUG
    float m_fLifeTimer = 0.0f;
    float m_fSpitTimer = 0.0f;
    
    bool m_bPrepareToSpit = false;
    bool m_bReadyToSpit = false;
    float m_fSpitReleaseTimer = 0.0f;

    bool m_bSpawned = false;

    const float m_fLifeSpan = 10.0f;
    const float m_fSpitFrequency = 3.0f;
    const float m_fSpitDelay = 0.5f;

};

#endif // SPIT_ZOMBIE_HPP
