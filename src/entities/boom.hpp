#ifndef BOOM_HPP
#define BOOM_HPP

#include "entity.hpp"
#include "animatable.hpp"
#include "rendarable.hpp"

enum class BoomType { Basic = 0, Acid, Dust };

class Boom : public Entity, public Animatable
{
public:
    Boom(const olc::vf2d& vPosition, BoomType type = BoomType::Basic)
        : Entity(vPosition),
          Animatable("./bin/res/booms_sheet.png", { { 0, 0 }, { 32, 32 }, { 64.f, 64.f } })
    {
        Animatable::LoadAnimation(
            {
                { 0, int(type) },
                { 1, int(type) },
                2,
                m_RenderData.vfRenderSize,
                m_RenderData.viFrameSize,
                true
            },
            "boom"
        );

        Animatable::AddAnimation("boom");
        Renderable::SetLayer(3);
    }

    void Update(float fElapsedTime) override {
        Animatable::Update(fElapsedTime);
    }

    void Render(olc::PixelGameEngine& pge) const override {
        Animatable::Render(m_vPosition, pge);
    }
};

#endif // BOOM_HPP
