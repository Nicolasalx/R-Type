/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EntityFactory
*/

#include "EntityFactory.hpp"
#include <fstream>
#include <utility>
#include "../utils/TrackedException.hpp"
#include "ClientEntityFactory.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "SpriteManager.hpp"
#include "components/beam.hpp"
#include "components/controllable.hpp"
#include "components/health.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/score.hpp"
#include "components/velocity.hpp"
#include "entity.hpp"
#include "imgui.h"
#include "shared_entity.hpp"
#include <imgui-SFML.h>

namespace ecs {

std::unordered_map<std::string, nlohmann::json> EntityFactory::jsonCache;

const nlohmann::json &EntityFactory::getJSON(const std::string &jsonFilePath)
{
    auto it = jsonCache.find(jsonFilePath);
    if (it != jsonCache.end()) {
        return it->second;
    }

    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        throw eng::TrackedException("Failed to open entity JSON file: " + jsonFilePath);
    }

    auto [insertedIt, _] = jsonCache.emplace(jsonFilePath, nlohmann::json::parse(file));
    return insertedIt->second;
}

entity_t EntityFactory::createClientEntityFromJSON(
    Registry &reg,
    SpriteManager &spriteManager,
    const std::string &jsonFilePath,
    int x,
    int y,
    shared_entity_t sharedEntity,
    float vx,
    float vy,
    std::shared_ptr<ImFont> font
)
{
    const nlohmann::json &entityJson = getJSON(jsonFilePath);

    std::string entityType = entityJson["type"].get<std::string>();
    bool isShared = entityType == "shared" || sharedEntity != std::numeric_limits<size_t>::max();
    entity_t entity = 0;

    if (isShared) {
        if (sharedEntity == std::numeric_limits<size_t>::max()) {
            sharedEntity = generateSharedEntityId();
        }
        if (reg.getLocalEntity().contains(sharedEntity)) {
            entity = reg.getLocalEntity().at(sharedEntity);
        } else {
            entity = reg.spawnSharedEntity(sharedEntity);
        }
    } else {
        entity = reg.spawnEntity();
    }

    ClientEntityFactory::addComponents(
        reg, spriteManager, entity, entityJson["components"], isShared, x, y, vx, vy, std::move(font)
    );

    return entity;
}

entity_t EntityFactory::createServerEntityFromJSON(
    Registry &reg,
    const std::string &jsonFilePath,
    int x,
    int y,
    shared_entity_t sharedEntity,
    float vx,
    float vy
)
{
    const nlohmann::json &entityJson = getJSON(jsonFilePath);

    std::string entityType = entityJson["type"].get<std::string>();
    bool isShared = entityType == "shared" || sharedEntity != std::numeric_limits<size_t>::max();
    entity_t entity = 0;

    if (isShared) {
        if (sharedEntity == std::numeric_limits<size_t>::max()) {
            sharedEntity = generateSharedEntityId();
        }
        if (reg.getLocalEntity().contains(sharedEntity)) {
            entity = reg.getLocalEntity().at(sharedEntity); // should never happen
        } else {
            entity = reg.spawnSharedEntity(sharedEntity);
        }
    } else {
        entity = reg.spawnEntity();
    }

    ServerEntityFactory::addComponents(reg, entity, entityJson["components"], isShared, x, y, vx, vy);
    ServerEntityFactory::handleNetworkSync(reg, entity, entityJson, isShared);

    return entity;
}

void EntityFactory::addCommonComponents(
    Registry &reg,
    entity_t entity,
    const nlohmann::json &componentsJson,
    int x,
    int y,
    float vx,
    float vy,
    std::shared_ptr<ImFont> font
)
{
    if (componentsJson.contains("position")) {
        auto posJson = componentsJson["position"];
        if (x != std::numeric_limits<int>::max()) {
            posJson["x"] = x;
        }
        if (y != std::numeric_limits<int>::max()) {
            posJson["y"] = y;
        }
        reg.addComponent(entity, ecs::component::Position{posJson["x"].get<float>(), posJson["y"].get<float>()});
    }

    if (componentsJson.contains("velocity")) {
        auto velJson = componentsJson["velocity"];
        if (vx != std::numeric_limits<float>::max()) {
            velJson["vx"] = vx;
        }
        if (vy != std::numeric_limits<float>::max()) {
            velJson["vy"] = vy;
        }
        reg.addComponent(entity, ecs::component::Velocity{velJson["vx"].get<float>(), velJson["vy"].get<float>()});
    }

    if (componentsJson.contains("hitbox")) {
        auto hitboxJson = componentsJson["hitbox"];
        reg.addComponent(
            entity, ecs::component::Hitbox{hitboxJson["width"].get<float>(), hitboxJson["height"].get<float>()}
        );
    }

    if (componentsJson.contains("controllable")) {
        reg.addComponent(entity, ecs::component::Controllable{});
    }

    if (componentsJson.contains("missile")) {
        int damage = 1;
        if (componentsJson["missile"].contains("damage")) {
            damage = componentsJson["missile"]["damage"].get<int>();
        }
        reg.addComponent(entity, ecs::component::Missile{damage});
    }

    if (componentsJson.contains("health")) {
        auto healthJson = componentsJson["health"];
        reg.addComponent(
            entity, ecs::component::Health{healthJson["maxHp"].get<int>(), healthJson["currHp"].get<int>()}
        );
    }

    if (componentsJson.contains("beam")) {
        reg.addComponent(entity, ecs::component::Beam{false, 0, false});
    }

    if (componentsJson.contains("score")) {
        float scoreValue = componentsJson["score"].get<float>();
        ecs::component::Score score;
        score.font = std::move(font);
        score.value = scoreValue;
        reg.addComponent(entity, ecs::component::Score{score});
    }

    if (componentsJson.contains("player")) {
        reg.addComponent(entity, ecs::component::Player{});
    }
}

} // namespace ecs
