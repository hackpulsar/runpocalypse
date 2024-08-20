#ifndef SPIT_ZOMBIE_HPP
#define SPIT_ZOMBIE_HPP

#include "zombie.hpp"

class SpitZombie : public Zombie
{
public:
    SpitZombie() : Zombie()
    {
        Animatable::LoadAnimation(
            {
                { 0, 1 },
                { 7, 1 },
                8,
                m_RenderData.vfRenderSize,
                m_RenderData.viFrameSize,
                false
            },
            "spit"
        );

        Animatable::AddAnimation("idle");
    }

    void Update(float fElapsedTime) override {
        Zombie::Update(fElapsedTime);

        if (!m_bEndGame) m_fShootTimer += fElapsedTime;

        if (m_bPrepareToSpit)
            m_fSpitReleaseTimer += fElapsedTime;

        if (m_fShootTimer >= m_fShootDelay) {
            m_fShootTimer = 0.0f;
            Animatable::AddAnimation("spit");
            m_bPrepareToSpit = true;
        }
        
        if (m_fSpitReleaseTimer >= m_fSpitDelay) {
            m_fSpitReleaseTimer = 0.0f;
            m_bPrepareToSpit = false;
            m_bReadyToShoot = true;
        }
        
        if (m_bSpawned) Animatable::Update(fElapsedTime);
    }
 
private:    
    bool m_bPrepareToSpit = false; 
    float m_fSpitReleaseTimer = 0.0f;

    const float m_fSpitDelay = 0.5f;

    
};

#endif // SPIT_ZOMBIE_HPP
