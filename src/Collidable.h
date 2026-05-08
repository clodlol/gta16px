#include <SFML/Graphics.hpp>

class Collidable
{
public:
    virtual ~Collidable() = default;
    virtual sf::FloatRect GetBounds() = 0;
};