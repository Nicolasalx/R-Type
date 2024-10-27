/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collideEffect
*/

#pragma once

#include <list>
#include "Registry.hpp"

namespace rts {

void collideEffect(ecs::Registry &reg, size_t entityA, size_t entityB, std::list<std::vector<char>> &datasToSend);

void resolveTagEffect(ecs::Registry &reg, size_t entityA, size_t entityB, std::list<std::vector<char>> &datasToSend);

} // namespace rts
