#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "collidable.hpp"

class Collision
{
public:
    static bool AABB(const Collidable& a, const Collidable& b) {
        Hitbox hitboxA = a.GetHitbox();
        Hitbox hitboxB = b.GetHitbox();

        return hitboxA.vPosition.x < hitboxB.vPosition.x + hitboxB.vSize.x &&
               hitboxA.vPosition.x + hitboxA.vSize.x > hitboxB.vPosition.x &&
               hitboxA.vPosition.y < hitboxB.vPosition.y + hitboxB.vSize.y &&
               hitboxA.vPosition.y + hitboxA.vSize.y > hitboxB.vPosition.y;    
    }
};

#endif // COLLISION_HPP
