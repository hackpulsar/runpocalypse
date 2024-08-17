#ifndef ROCKET_HPP
#define ROCKET_HPP

#include "animatable.hpp"
#include "entity.hpp"

class Rocket : public Entity, Animatable
{
public:
    Rocket(olc::vi2d vPosition = { 0, 0 }) 
        : Entity(vPosition)
    {
        m_pSpritesheet = std::make_unique<olc::Sprite>("../res/rocket_sheet.png");
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
        Animatable::Update(fElapsedTime);
    }
    
    void Render(olc::PixelGameEngine& pge) const override {
        Animatable::Render(m_vPosition, pge);
    }

};

#endif // ROCKET_HPP
