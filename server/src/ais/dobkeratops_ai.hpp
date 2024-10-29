/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** dobkeratops_ai
*/

#pragma once

#include <cmath>
#include <list>
#include <vector>
#include "Registry.hpp"
#include "components/dobkeratops.hpp"
#include "entity.hpp"

namespace rts::ais {

void initDobkeratopsAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<std::vector<char>> &datasToSend,
    Dobkeratops &state
);

void dobkeratopsAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<std::vector<char>> &datasToSend,
    Dobkeratops &state
);

} // namespace rts::ais
