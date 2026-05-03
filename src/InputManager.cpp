#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "InputManager.h"

bool InputManager::IsActionActive(const std::string &action)
{
    return actionMap.count(action) ? actionMap[action] : false;
}

void InputManager::Update()
{
    // Straight Movement
    actionMap["MoveLeft"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    actionMap["MoveRight"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    actionMap["MoveUp"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    actionMap["MoveDown"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);

    // Diagonal Movement
    actionMap["MoveUpLeft"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    actionMap["MoveUpRight"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    actionMap["MoveDownLeft"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    actionMap["MoveDownRight"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
}