#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "olcPixelGameEngine.h"

class Entity
{
public:
    Entity(olc::vi2d vPosition = { 0, 0 }) 
        : m_vPosition(vPosition)
    { }

    virtual void HandleInput(olc::PixelGameEngine& pge) { };
    virtual void Update(float fElapsedTime) = 0; 
    virtual void Render(olc::PixelGameEngine& pge) const = 0;

protected:
    olc::vi2d m_vPosition;

};

#endif // ENTITY_HPP
