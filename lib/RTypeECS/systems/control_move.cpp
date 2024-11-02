/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_move
*/

#include "control_move.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "InputManager.hpp"
#include "KeyBind.hpp"
#include "RTypeConst.hpp"
#include "Registry.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Zipper.hpp"
#include "components/controllable.hpp"
#include "components/velocity.hpp"

namespace ecs::systems {

void controlMove(
    Registry &reg,
    ecs::InputManager &input,
    const ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
)
{
    auto &velocities = reg.getComponents<ecs::component::Velocity>();
    auto &controllables = reg.getComponents<ecs::component::Controllable>();

    sf::Vector2f direction(0.f, 0.f);
    if (input.isKeyPressed(keyBind.getActionKey(rt::PlayerAction::MOVE_UP))) {
        direction.y -= 1.f;
    }
    if (input.isKeyPressed(keyBind.getActionKey(rt::PlayerAction::MOVE_DOWN))) {
        direction.y += 1.f;
    }
    if (input.isKeyPressed(keyBind.getActionKey(rt::PlayerAction::MOVE_LEFT))) {
        direction.x -= 1.f;
    }
    if (input.isKeyPressed(keyBind.getActionKey(rt::PlayerAction::MOVE_RIGHT))) {
        direction.x += 1.f;
    }

    float speed = 150.0f;

    Zipper<ecs::component::Velocity, ecs::component::Controllable> zip(velocities, controllables);

    for (auto [vel, _] : zip) {
        vel.vx = direction.x * speed;
        vel.vy = direction.y * speed;
    }
}

} // namespace ecs::systems
