// #include <iostream>
// #include <type_traits>

// #include "Weapon.h"
// #include "InputManager.h"

// #define TILE_SIZE 225

// template <typename T>
// Weapon<T>::Weapon(float vel, float rate, int dmg) : velocity{vel}, fireRate{rate}, damage{dmg}
// {
//     cooldownTimer = 1.f / fireRate;
// }

// template <typename T>
// void Weapon<T>::Load(std::filesystem::path filePath)
// {
//     if (!projectileTexture.loadFromFile(filePath))
//     {
//         std::cout << "Failed to load texture located at " << filePath << "\n";
//         return;
//     }
// }

// template <typename T>
// void Weapon<T>::Fire(const T &projectile)
// {

//     currentProjectile = projectile;

//     firing = true;
// }

// template <typename T>
// void Weapon<T>::StopFire()
// {
//     firing = false;
// }

// template <typename T>
// const std::vector<T> &Weapon<T>::GetProjectiles() const
// {
//     return projectiles;
// }

// template <typename T>
// const sf::Texture &Weapon<T>::GetProjectileTexture() const
// {
//     return projectileTexture;
// }

// template <typename T>
// void Weapon<T>::Update(float deltaTime)
// {
//     cooldownTimer -= deltaTime;

//     if (firing && cooldownTimer <= 0.f)
//     {
//         cooldownTimer = 1 / fireRate;

//         projectiles.push_back(currentProjectile);
//     }

//     projectiles.erase(
//         std::remove_if(projectiles.begin(), projectiles.end(),
//                        [&](T &proj)
//                        {
//                            if (proj.GetSprite().getPosition().x <= 0 && proj.GetSprite().getPosition().x >= 1600)
//                                return true;

//                            if (proj.GetSprite().getPosition().y <= 0 && proj.GetSprite().getPosition().y >= 1600)
//                                return true;

//                            proj.MoveSprite({cos(proj.GetDirection().asRadians()) * velocity * deltaTime, sin(proj.GetDirection().asRadians()) * velocity * deltaTime})

//                                return false;
//                        }),
//         projectiles.end());
//     // what the actual fuck is this
// }

// template <typename T>
// void Weapon<T>::Draw(sf::RenderWindow &window)
// {
//     for (const Bullet &bullet : bullets)
//         window.draw(bullet.sprite);
// }