#pragma once

#include <unordered_map>

class InputManager
{
public:
    bool IsActionActive(const std::string &action);
    sf::Vector2f GetMousePosition(); // gets mouse position relative to center of the window
    void Update(sf::RenderWindow &window);

private:
    std::unordered_map<std::string, bool> actionMap{};
    sf::Vector2f mousePos;
};