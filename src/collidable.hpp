#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

#include "olcPixelGameEngine.h"

struct Hitbox
{
    olc::vf2d vPosition;
    olc::vf2d vSize;
};

class Collidable
{
public:
    Collidable(const Hitbox& hitbox)
        : m_Hitbox(hitbox)
    { }

    Hitbox GetHitbox() const { return m_Hitbox; }

    void UpdatePosition(const olc::vf2d& vPosition) {
        m_Hitbox.vPosition = vPosition;
    }

protected:
    Hitbox m_Hitbox;

};

#endif // COLLIDABLE_HPP
