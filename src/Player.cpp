#include <iostream>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "InputManager.h"

#define TILE_SIZE 59

sf::FloatRect Player::GetBounds()
{
    return playerSprite.getGlobalBounds();
}

Player::Player() : playerTexture{}, playerSprite{playerTexture}
{
}

const sf::Sprite &Player::GetSprite()
{
    return playerSprite;
}

void Player::TakeDamage(int sourceDamage)
{
    health -= (sourceDamage - sourceDamage * (defense / 100));
}

void Player::Load()
{
    if (!playerTexture.loadFromFile("../assets/player.png"))
    {
        std::cout << "Failed to load player texture\n";
        return;
    }

    playerSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE}));

    playerSprite.setPosition({1000.f, 1000.f});
    playerSprite.setOrigin({TILE_SIZE / 2, TILE_SIZE / 2});
    playerSprite.setScale({16.f / TILE_SIZE, 16.f / TILE_SIZE});

    gun.Load();
}

void Player::Update(float deltaTime, InputManager &input, sf::View &camera)
{
    sf::Vector2f direction{0.f, 0.f};

    if (input.IsActionActive("MoveLeft"))
    {
        direction.x -= 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE * 1}, {TILE_SIZE, TILE_SIZE}));
    }
    if (input.IsActionActive("MoveRight"))
    {
        direction.x += 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE * 2}, {TILE_SIZE, TILE_SIZE}));
    }
    if (input.IsActionActive("MoveUp"))
    {
        direction.y -= 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE * 3}, {TILE_SIZE, TILE_SIZE}));
    }
    if (input.IsActionActive("MoveDown"))
    {
        direction.y += 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE * 0}, {TILE_SIZE, TILE_SIZE}));
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
        gun.Fire(playerSprite.getPosition(), input.GetMousePosition().angle(), "Player");
    else
        gun.StopFire();

    gun.Update(deltaTime, camera);
}

void Player::Draw(sf::RenderWindow &window)
{
    window.draw(playerSprite);
    gun.Draw(window);
}