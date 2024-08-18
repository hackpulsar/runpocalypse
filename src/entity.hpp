#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "olcPixelGameEngine.h"

class Entity
{
public:
    Entity(olc::vf2d vPosition = { 0.f, 0.f }) 
        : m_vPosition(vPosition)
    { }

    virtual ~Entity() = default;

    virtual void HandleInput(olc::PixelGameEngine& pge) { };
    virtual void Update(float fElapsedTime) = 0; 
    virtual void Render(olc::PixelGameEngine& pge) const = 0;

    virtual void AdjustPosition(const int nLane) { m_nLane = nLane; };

    void SelfDestruct() { m_bSelfDestruct = true; }

    bool IsSelfDestruct() const { return m_bSelfDestruct; }
    olc::vf2d GetPosition() const { return m_vPosition; }
    int GetLane() const { return m_nLane; }

protected:
    olc::vf2d m_vPosition;
    int m_nLane;

    bool m_bSelfDestruct = false;

};

#endif // ENTITY_HPP
