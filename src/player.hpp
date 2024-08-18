#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"
#include "animatable.hpp"
#include "collidable.hpp"

class Player : public Entity, Animatable, public Collidable 
{
public:
    Player()
        : Entity({ float(OFFSET_TO_LANE * SCALE + (MIDDLE_LINE_WIDTH + 1) * SCALE), 80.f }),
          Collidable({ { float(OFFSET_TO_LANE * SCALE + (MIDDLE_LINE_WIDTH + 1) * SCALE), 80.f }, { 25.f, 25.f } })
    {
        m_pSpritesheet = std::make_unique<olc::Sprite>("./bin/res/player_sheet.png");
        m_pDecal = std::make_unique<olc::Decal>(m_pSpritesheet.get());
        m_nLane = 1;

        Animatable::LoadAnimation(
            {
                { 1, 0 },
                { 2, 0 },
                2,
                { 25.f, 50.f },
                { 16, 32 },
                true
            }, 
            "run"
        );

        Animatable::AddAnimation("run");
    }

    void HandleInput(olc::PixelGameEngine& pge) override {
        if (pge.GetKey(olc::Key::LEFT).bPressed && m_nLane > 1) {
            if (m_nLane == 3)
                m_vPosition.x -= (LANE_WIDTH + MIDDLE_LINE_WIDTH) * SCALE;
            else
                m_vPosition.x -= LANE_WIDTH * SCALE;
            m_nLane--;
        }
        if (pge.GetKey(olc::Key::RIGHT).bPressed && m_nLane < 4) {
            if (m_nLane == 2)
                m_vPosition.x += (LANE_WIDTH + MIDDLE_LINE_WIDTH) * SCALE;
            else
                m_vPosition.x += LANE_WIDTH * SCALE;
            m_nLane++;
        }
    }

    void Update(float fElapsedTime) override {
        Animatable::Update(fElapsedTime);
        Collidable::UpdatePosition(m_vPosition);
    }

    void Render(olc::PixelGameEngine& pge) const override {
        Animatable::Render(m_vPosition, pge);
    }

    void Reset() {
        m_vPosition = { float(OFFSET_TO_LANE * SCALE + (MIDDLE_LINE_WIDTH + 1) * SCALE), 80.f };
        Collidable::UpdatePosition(m_vPosition);
        m_nLane = 1;
    }

private:
    float m_fAnimationDelay = 0.0f;
    bool m_nFrame = 1;

};

#endif // PLAYER_HPP
