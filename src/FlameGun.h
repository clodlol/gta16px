#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Collidable.h"
#include "utils.h"

#define TILE_SIZE_FLAME 400
#define PI 3.14159

class FlameProjectile : public Collidable
{

public:
    FlameProjectile(const sf::Texture &tex, const sf::Vector2f &ogn, const sf::Vector2f &dir, int dmg, float vel, float maxDist) : sprite(tex), direction(dir), origin(ogn), damage(dmg), velocity(vel), maxDistance(maxDist)
    {
        sprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_FLAME, TILE_SIZE_FLAME}));
        sprite.setOrigin({TILE_SIZE_FLAME / 2, TILE_SIZE_FLAME / 2});
        sprite.scale({8.f / TILE_SIZE_FLAME, 8.f / TILE_SIZE_FLAME});
        sprite.rotate(direction.angle());
        sprite.setPosition(origin);
    }

    sf::FloatRect GetBounds() const override { return sprite.getGlobalBounds(); }
    const sf::Sprite &GetSprite() const override { return sprite; }

    const sf::Vector2f &GetOrigin() const { return origin; }
    const sf::Vector2f &GetDirection() const { return direction; }

    int GetDamage() const { return damage; }
    float GetVelocity() const { return velocity; }

    void Erase() { expired = true; }

    bool IsExpired() const { return expired; }

    friend class FlameGun;

private:
    sf::Sprite sprite;

    sf::Vector2f origin{1.f, 1.f};
    sf::Vector2f direction{1.f, 1.f};

    bool expired = false;

    int damage;
    float velocity;
    float maxDistance;
};

class FlameGun
{
public:
    FlameGun(float vel, float rate, int dmg, int inty, float spr) : velocity(vel), fireRate(rate), damage(dmg), intensity(inty), spread(spr)
    {
        cooldownTimer = 1.f / fireRate;
    }

    void Fire(const FlameProjectile &projectile)
    {
        currentProjectile = projectile;
        firing = true;
    }

    void StopFire()
    {
        firing = false;
    }

    std::vector<FlameProjectile> &GetProjectiles() { return projectiles; }
    const sf::Texture &GetProjectileTexture() const { return flameProjectileTexture; }

    int GetDamage() const { return damage; }

    void Load()
    {
        if (!flameProjectileTexture.loadFromFile("../assets/flame.gif"))
        {
            std::cout << "Error while loading flame gun projectile texture\n";
            return;
        }
    }

    void Update(float deltaTime)
    {
        cooldownTimer -= deltaTime;

        if (firing && cooldownTimer <= 0.f)
        {
            cooldownTimer = 1 / fireRate;

            for (int i = 0; i < intensity; ++i)
            {
                float randomAngle = currentProjectile.direction.angle().asRadians() + (randomNumGen(-PI / 10.f * spread, PI / 10.f * spread));
                currentProjectile.direction = {cos(randomAngle), sin(randomAngle)};
                currentProjectile.velocity = randomNumGen(0.1, currentProjectile.velocity);
                projectiles.push_back(currentProjectile);
            }
        }

        for (FlameProjectile &proj : projectiles)
        {
            proj.sprite.move({cos(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime, sin(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime});

            if ((proj.origin - proj.sprite.getPosition()).length() > proj.maxDistance)
            {
                proj.expired = true;
            }

            // bounds checking is optional since flames die anyways
        }

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](FlameProjectile &proj)
                           {
                               return proj.IsExpired();
                           }),
            projectiles.end());
    }

    void Draw(sf::RenderWindow &window)
    {
        for (const FlameProjectile &projectile : projectiles)
            window.draw(projectile.GetSprite());
    }

private:
    sf::Texture flameProjectileTexture{};
    FlameProjectile currentProjectile{flameProjectileTexture, sf::Vector2f{1.f, 1.f}, sf::Vector2f{1.f, 1.f}, 0, 1.f, 1.f};
    std::vector<FlameProjectile> projectiles;

    float velocity = 1.f;
    float fireRate = 1.f; // bullets per second
    int damage = 0;
    int intensity = 3;    // how many flames per burst
    float spread = 0.25f; // how much spread, float from 0 to 1

    bool firing = false;
    float cooldownTimer = 1 / fireRate;
};
