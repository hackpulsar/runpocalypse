#ifndef BOOM_HPP
#define BOOM_HPP

#include "entity.hpp"
#include "animatable.hpp"

enum class BoomType { Basic = 0, Acid };

class Boom : public Entity, Animatable
{
public:
    Boom(const olc::vf2d& vPosition, BoomType type = BoomType::Basic)
        : Entity(vPosition)
    {
        m_pSpritesheet = std::make_unique<olc::Sprite>("./bin/res/booms_sheet.png");
        m_pDecal = std::make_unique<olc::Decal>(m_pSpritesheet.get());

        Animatable::LoadAnimation(
            {
                { 0, int(type) },
                { 1, int(type) },
                2,
                { 64.f, 64.f },
                { 32, 32 },
                true
            },
            "boom"
        );

        Animatable::AddAnimation("boom");
    }

    void Update(float fElapsedTime) override {
        Animatable::Update(fElapsedTime);
    }

    void Render(olc::PixelGameEngine& pge) const override {
        Animatable::Render(m_vPosition, pge);
    }
};

#endif // BOOM_HPP
