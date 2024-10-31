/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** waveAi
*/

#include <cmath>
#include <cstdio>
#include <list>

#include "Registry.hpp"
#include "WaveManager.hpp"

namespace rts::ais {

void splitAi(ecs::Registry &reg, entity_t e, std::list<std::vector<char>> &datasToSend, ecs::WaveManager &waveManager);

} // namespace rts::ais
