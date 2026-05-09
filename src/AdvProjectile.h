#pragma once
#include "Projectile.h"

class AdvancedProjectile : public Projectile {

public:
    virtual ~AdvancedProjectile() = default;

    virtual void Update(float deltaTime) {}
    virtual bool IsExpired() const{
        return false;
    }
};
