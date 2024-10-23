/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_special
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include "InputManager.hpp"
#include "KeyBind.hpp"
#include "RTypeConst.hpp"
#include "Registry.hpp"
#include "udp/UDPClient.hpp"

namespace ecs::systems {

/**
 * @brief Handles entity control and input processing.
 *
 * The `control_special` function interprets the input state from @param input
 * which is an object that contain all the inputs states.
 *
 * This function is analog to `control` function that handle movements inputs, and handle
 * special keyboard input.
 *
 * @param reg Reference to the registry managing entities and their components.
 * @param input Input manager object that contains states of keyboard values.
 * @param udp Udp client to send informations to server.
 */

void controlSpecial(Registry &reg, ntw::UDPClient &udp);

} // namespace ecs::systems
