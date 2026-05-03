#include <iostream>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "InputManager.h"

#define TILE_SIZE 59

Player::Player() : playerTexture{}, playerSprite{playerTexture}
{
}

Player::~Player()
{
}
void Player::Initialize()
{
    speed = 50;
    health = 100;

    playerSprite.setPosition({1000.f, 1000.f});
    playerSprite.setOrigin({TILE_SIZE / 2, TILE_SIZE / 2});
    playerSprite.setScale({16.f / TILE_SIZE, 16.f / TILE_SIZE});
}

void Player::Load()
{
    if (!playerTexture.loadFromFile("../assets/player.png"))
    {
        std::cout << "Failed to load player texture\n";
        return;
    }

    playerSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE}));
}

void Player::Update(float deltaTime, InputManager &input, sf::View &camera)
{
    sf::Vector2f movement{0.f, 0.f};

    if (input.IsActionActive("MoveLeft"))
        movement.x -= 1;
    if (input.IsActionActive("MoveRight"))
        movement.x += 1;
    if (input.IsActionActive("MoveUp"))
        movement.y -= 1;
    if (input.IsActionActive("MoveDown"))
        movement.y += 1;

    float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length > 0)
    {
        movement /= length;
    }

    sf::Vector2f newPos = playerSprite.getPosition() + movement;
    playerSprite.setPosition({std::clamp(newPos.x, 0.f, 1600.f), std::clamp(newPos.y, 0.f, 1600.f)});

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
}

void Player::Draw(sf::RenderWindow &window)
{
    window.draw(playerSprite);
}