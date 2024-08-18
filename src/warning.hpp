#ifndef WARNING_HPP
#define WARNING_HPP

#include "entity.hpp"
#include "animatable.hpp"
#include "settings.hpp"

class Warning : public Entity, Animatable
{
public:
    Warning()
        : Entity({ 0.f, 0.f })
    {
        m_pSpritesheet = std::make_unique<olc::Sprite>("./bin/res/warning_sheet.png");
        m_pDecal = std::make_unique<olc::Decal>(m_pSpritesheet.get());
        
        Animatable::LoadAnimation(
            {
                { 0, 0 },
                { 1, 0 },
                2,
                { 8.f, 32.f },
                { 16, 64 },
                true
            },
            "flash"
        );

        Animatable::AddAnimation("flash");
    }

    void Update(float fElapsedTime) override {
        Animatable::Update(fElapsedTime);

        m_fTimer += fElapsedTime;
        if (m_fTimer >= WARNING_LASTS)
            m_bSelfDestruct = true;
    }

    void Render(olc::PixelGameEngine& pge) const override {
        Animatable::Render(m_vPosition, pge);
    }

    void AdjustPosition(const int nLane) override {
        if (nLane > 2)
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + MIDDLE_LINE_WIDTH + (LANE_WIDTH / 2 - 4)) * SCALE;
        else
            m_vPosition.x = (OFFSET_TO_LANE + (nLane - 1) * LANE_WIDTH + (LANE_WIDTH / 2 - 2)) * SCALE;
        m_vPosition.y = SCREEN_HEIGHT - 32 * SCALE;
    }

private:
    float m_fTimer = 0.0f;

};

#endif // WARNING_HPP
