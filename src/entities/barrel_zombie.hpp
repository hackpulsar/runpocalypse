#ifndef BARREL_ZOMBIE_HPP
#define BARREL_ZOMBIE_HPP

#include "zombie.hpp"

class BarrelZombie : public Zombie
{
public:
    BarrelZombie() : Zombie()
    {
        Animatable::LoadAnimation(
            {
                { 0, 2 },
                { 1, 2 },
                2,
                m_RenderData.vfRenderSize,
                m_RenderData.viFrameSize,
                true
            },
            "loaded"
        ); 

        Animatable::AddAnimation("loaded");    
    }

    void Update(float fElapsedTime) override {
        Zombie::Update(fElapsedTime);

        if (!m_bShot) m_fShootTimer += fElapsedTime;

        if (m_fShootTimer >= m_fThrowDelay) {
            m_fShootTimer = 0.0f;
            m_bReadyToShoot = true;

            Animatable::PopAnimation();
            Animatable::AddAnimation("idle");
        }

        if (m_bSpawned) Animatable::Update(fElapsedTime);
    }

    void Shoot() override {
        Zombie::Shoot();
        m_bShot = true;
    }

private:
    bool m_bShot = false;
    const float m_fThrowDelay = 5.0f;

};

#endif // BARREL_ZOMBIE_HPP
