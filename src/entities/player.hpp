#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"
#include "animatable.hpp"
#include "collidable.hpp"
#include "rendarable.hpp"

#include "log.hpp"

class Player : public Entity, public Animatable, public Collidable 
{
public:
    Player(bool* pGameRunning)
        : Entity({ float(OFFSET_TO_LANE * SCALE + (MIDDLE_LINE_WIDTH + 1) * SCALE), 80.f }),
          Animatable("./bin/res/player_sheet.png", { { 0, 0 }, { 16, 32 }, { 25.f, 50.f } }),
          Collidable({ { float(OFFSET_TO_LANE * SCALE + (MIDDLE_LINE_WIDTH + 1) * SCALE), 80.f }, { 25.f, 2.f } }),
          m_pGameRunning(pGameRunning)
    {
        m_nLane = 1;

        Animatable::LoadAnimation(
            {
                { 1, 0 },
                { 2, 0 },
                2,
                m_RenderData.vfRenderSize,
                m_RenderData.viFrameSize,
                true
            }, 
            "run"
        );

        Animatable::AddAnimation("run");

        Renderable::SetLayer(1); 
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
        Collidable::UpdatePosition(m_vPosition + olc::vf2d(0.f, 48.f));
    }

    void Render(olc::PixelGameEngine& pge) const override {
        if (*m_pGameRunning) Animatable::Render(m_vPosition, pge);
    }

    void Reset() {
        m_vPosition = { float(OFFSET_TO_LANE * SCALE + (MIDDLE_LINE_WIDTH + 1) * SCALE), 80.f };
        Collidable::UpdatePosition(m_vPosition);
        m_nLane = 1;

        LOG("Player reset");
    }

private:
    float m_fAnimationDelay = 0.0f;
    bool m_nFrame = 1;

    bool* m_pGameRunning;

};

#endif // PLAYER_HPP
