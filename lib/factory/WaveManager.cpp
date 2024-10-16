/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** WaveManager
*/

#include "WaveManager.hpp"
#include <algorithm>
#include "entity.hpp"

size_t ecs::WaveManager::addNewWave()
{
    _waves.push_back({});
    return _waves.size() - 1;
}

void ecs::WaveManager::addNewMob(size_t waveId, const ecs::WaveManager::mob_spawner_t &spawner)
{
    _waves[waveId].push_back(spawner);
}

void ecs::WaveManager::removeEntity(entity_t entity)
{
    auto e = std::find(_currEntities.begin(), _currEntities.end(), entity);

    if (e == _currEntities.end()) {
        return;
    }
    _currEntities.erase(e);
}

void ecs::WaveManager::spawnNextWave(ecs::Registry &reg)
{
    auto wave = _waves[_nextWave];

    for (auto &spawner : wave) {
        _currEntities.push_back(spawner(reg));
    }
    _nextWave++;
}

bool ecs::WaveManager::isMob(entity_t entity)
{
    return std::find(_currEntities.begin(), _currEntities.end(), entity) != _currEntities.end();
}
