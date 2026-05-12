#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <type_traits>

#include "InputManager.h"
#include "Collidable.h"

#define TILE_SIZE_ROCKET 260

#include <SFML/Graphics.hpp>

class RocketBullet : public Collidable
{
public:
    RocketBullet(const sf::Texture &tex, const sf::Vector2f &ogn, const sf::Vector2f &dir, int dmg, float vel, float expTime, float bRadius, float bTime) : sprite(tex), damage(dmg), velocity(vel), origin(ogn), direction(dir), explosionTime(expTime), blastRadius(bRadius), blastTime(bTime)
    {
        sprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_ROCKET, TILE_SIZE_ROCKET}));
        sprite.setOrigin({TILE_SIZE_ROCKET / 2, TILE_SIZE_ROCKET / 2});
        sprite.scale({8.f / TILE_SIZE_ROCKET, 8.f / TILE_SIZE_ROCKET});
        sprite.rotate(sf::degrees(direction.angle().asDegrees() + 90)); // special for this asset because bullet facing upwards hai isme
        sprite.setPosition(origin);

        explosion.setRadius(0);
        explosion.setOrigin({0, 0});
        explosion.setFillColor(sf::Color(255, 100, 0, 180));
        explosion.setPosition(sprite.getPosition());
    }

    sf::FloatRect GetBounds() const override
    {
        if (exploded)
            return explosion.getGlobalBounds();

        return sprite.getGlobalBounds();
    }
    const sf::Sprite &GetSprite() const override { return sprite; }

    const sf::Vector2f &GetOrigin() const { return origin; }
    const sf::Vector2f &GetDirection() const { return direction; }

    int GetDamage() const { return damage; }
    float GetVelocity() const { return velocity; }

    void Explode()
    {
        exploded = true;
        blastTimer = 0.f;
        explosion.setPosition(sprite.getPosition());
    }

    bool IsExploded() const { return exploded; }

    bool IsExpired() const { return expired; }

    friend class RocketGun;

private:
    sf::Sprite sprite;
    sf::CircleShape explosion;

    sf::Vector2f origin{1.f, 1.f};
    sf::Vector2f direction{1.f, 1.f};

    bool exploded = false;
    bool expired = false;
    float timer = 0.f;
    float blastTimer = 0.f;

    float explosionTime;
    float blastTime;
    int damage;
    float velocity;
    float blastRadius;
};

class RocketGun
{

public:
    RocketGun(float vel, float rate, int dmg) : velocity(vel), fireRate(rate), damage(dmg)
    {
        cooldownTimer = 1.f / fireRate;
    }

    void Fire(const RocketBullet &projectile)
    {
        currentProjectile = projectile;
        firing = true;
    }

    void StopFire()
    {
        firing = false;
    }

    std::vector<RocketBullet> &GetProjectiles() { return projectiles; }
    const sf::Texture &GetProjectileTexture() const { return projectileTexture; }
    int GetDamage() const { return damage; }

    void Load()
    {
        if (!projectileTexture.loadFromFile("../assets/rocket.png"))
        {
            std::cout << "Failed to load rocket texture\n";
            return;
        }
    }

    void Update(float deltaTime)
    {
        cooldownTimer -= deltaTime;

        if (firing && cooldownTimer <= 0.f)
        {
            cooldownTimer = 1 / fireRate;

            projectiles.push_back(currentProjectile);
        }

        for (RocketBullet &proj : projectiles)
        {
            proj.timer += deltaTime;
            proj.blastTimer += deltaTime;

            proj.sprite.move({cos(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime, sin(proj.GetDirection().angle().asRadians()) * (velocity + proj.GetVelocity()) * deltaTime});

            if (proj.timer > proj.explosionTime)
            {
                if (!proj.IsExploded())
                    proj.Explode();
            }

            if ((proj.sprite.getPosition().x <= 0 || proj.sprite.getPosition().x >= 1600))
            {
                if (!proj.IsExploded())
                    proj.Explode();
            }

            if (proj.sprite.getPosition().y <= 0 || proj.sprite.getPosition().y >= 1600)
            {
                if (!proj.IsExploded())
                    proj.Explode();
            }

            if (proj.IsExploded())
            {
                float currRad = proj.blastRadius * (proj.blastTimer / proj.blastTime);

                proj.explosion.setRadius(currRad);
                proj.explosion.setOrigin({currRad, currRad});

                if (proj.blastTimer / proj.blastTime >= 1)
                {
                    proj.expired = true;
                }

                int alpha = static_cast<int>(255 - 255 * (proj.blastTimer / proj.blastTime));
                proj.explosion.setFillColor(sf::Color(255, 100, 0, alpha));
            }
        }

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](RocketBullet &proj)
                           {
                               return proj.IsExpired();
                           }),
            projectiles.end());
    }

    void Draw(sf::RenderWindow &window)
    {
        for (const RocketBullet &projectile : projectiles)
        {
            if (projectile.IsExploded())
            {
                window.draw(projectile.explosion);
            }
            else
            {
                window.draw(projectile.sprite);
            }
        }
    }

private:
    sf::Texture projectileTexture{};
    RocketBullet currentProjectile{projectileTexture, sf::Vector2f{1.f, 1.f}, sf::Vector2f{1.f, 1.f}, 0, 1.f, 1.f, 2.f, 1.f};
    std::vector<RocketBullet> projectiles;

    float velocity = 1.f;
    float fireRate = 1.f; // bullets per second
    int damage = 0;

    bool firing = false;
    float cooldownTimer = 1 / fireRate;
};