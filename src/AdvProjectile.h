#pragma once

class AdvancedProjectile {

public:
    virtual ~AdvancedProjectile() = default;

    //flames max allowed distance through these
    virtual float GetDistanceTravelled() const = 0;

    virtual float GetMaxDistance() const = 0;

    virtual float AddDistance(float distance) const = 0;

    //handles the flame expiration
    virtual bool IsExpired() const{
        return GetDistanceTravelled() >= GetMaxDistance();
    }
};
