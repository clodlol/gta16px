#include <iostream>
#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "InputManager.h"

bool InputManager::IsActionActive(const std::string &action)
{
    return actionMap.count(action) ? actionMap[action] : false;
}

sf::Vector2f &InputManager::GetMousePosition()
{
    return mousePos;
}

void InputManager::Update(sf::RenderWindow &window, sf::View &camera)
{
    // Movement
    actionMap["MoveLeft"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    actionMap["MoveRight"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    actionMap["MoveUp"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    actionMap["MoveDown"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);

    // Camera
    actionMap["ZoomIn"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add);
    actionMap["ZoomOut"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract);

    // Fire
    actionMap["Fire"] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // calculations for mouse pos
    sf::Vector2f worldMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    mousePos = worldMouse - camera.getCenter();

    // std::cout << "Mouse X: " << mousePos.x << " Mouse Y: " << mousePos.y << "\n";

    actionMap["Hero0"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1);
    actionMap["Hero1"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2);
    actionMap["Hero2"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3);
    actionMap["Hero3"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4);
}