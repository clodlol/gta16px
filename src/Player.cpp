#include <iostream>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "InputManager.h"

#define TILE_SIZE_PLAYER 59

Player::Player(const sf::Vector2f &spwnPos) : playerTexture{}, playerSprite{playerTexture}, spawnLocation{spwnPos}
{
    immunityTimer = immunityTime;
    deathTimer = respawnTime;
}

sf::FloatRect Player::GetBounds() const
{
    return playerSprite.getGlobalBounds();
}

const sf::Sprite &Player::GetSprite() const
{
    return playerSprite;
}

void Player::TakeDamage(int sourceDamage)
{
    if (immunityTimer <= 0.f && alive)
    {
        int damageTaken = sourceDamage - float(sourceDamage * defense / 100.f);

        health -= damageTaken;

        std::cout << "Player took " << damageTaken << " damage, current health: " << health << "\n";

        immunityTimer = immunityTime;
    }

    if (health <= 0)
    {
        alive = false;
        deathTimer = respawnTime;
    }
}

void Player::Load()
{
    if (!playerTexture.loadFromFile("../assets/player.png"))
    {
        std::cout << "Failed to load player texture\n";
        return;
    }

    playerSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));

    playerSprite.setPosition({1000.f, 1000.f});
    playerSprite.setOrigin({TILE_SIZE_PLAYER / 2, TILE_SIZE_PLAYER / 2});
    playerSprite.setScale({16.f / TILE_SIZE_PLAYER, 16.f / TILE_SIZE_PLAYER});

    gun.Load("../assets/bullet.png");
}

void Player::Update(float deltaTime, InputManager &input, sf::View &camera)
{
    immunityTimer -= deltaTime;

    if (!alive)
    {
        deathTimer -= deltaTime;
        if (deathTimer <= 0.f)
        {
            // respawn and reset wanted level
            alive = true;
            health = 100;
            defense = 50;
            deathTimer = respawnTime;
        }
        else
            return;
    }

    sf::Vector2f direction{0.f, 0.f};

    if (input.IsActionActive("MoveLeft"))
    {
        direction.x -= 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE_PLAYER * 1}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));
    }
    if (input.IsActionActive("MoveRight"))
    {
        direction.x += 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE_PLAYER * 2}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));
    }
    if (input.IsActionActive("MoveUp"))
    {
        direction.y -= 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE_PLAYER * 3}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));
    }
    if (input.IsActionActive("MoveDown"))
    {
        direction.y += 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE_PLAYER * 0}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));
    }

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0)
    {
        direction /= length;
    }

    sf::Vector2f movement = direction * speed * deltaTime;

    sf::Vector2f currentPos = playerSprite.getPosition();
    playerSprite.setPosition({std::clamp(currentPos.x + movement.x, 0.f, 1600.f),
                              std::clamp(currentPos.y + movement.y, 0.f, 1600.f)});

    float newCamTopEdge = playerSprite.getPosition().y - camera.getSize().y / 2;
    float newCamLeftEdge = playerSprite.getPosition().x - camera.getSize().x / 2;
    float newCamRightEdge = playerSprite.getPosition().x + camera.getSize().x / 2;
    float newCamBotEdge = playerSprite.getPosition().y + camera.getSize().y / 2;

    sf::Vector2f newCamCenter = camera.getCenter();
    if (newCamLeftEdge > 0.f && newCamRightEdge < 1600.f)
    {
        newCamCenter.x = playerSprite.getPosition().x;
    }
    if (newCamTopEdge > 0.f && newCamBotEdge < 1600.f)
    {
        newCamCenter.y = playerSprite.getPosition().y;
    }

    camera.setCenter(newCamCenter);

    if (input.IsActionActive("Fire"))
        gun.Fire(Bullet{gun.GetProjectileTexture(), playerSprite.getPosition(), input.GetMousePosition(), bulletDamage, bulletVelocity});
    else
        gun.StopFire();

    gun.Update(deltaTime);
}

void Player::Draw(sf::RenderWindow &window)
{
    if (alive)
    {
        window.draw(playerSprite);
        gun.Draw(window);
    }
}