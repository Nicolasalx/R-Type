/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ServerEntityFactory
*/

#include "ServerEntityFactory.hpp"
#include "components/animation.hpp"
#include "components/dobkeratops.hpp"
#include "components/drawable.hpp"
#include "components/tag.hpp"
#include "components/ai_actor.hpp"
#include "components/game_tag.hpp"
#include "components/server_share_movement.hpp"

namespace ecs {

void ServerEntityFactory::addComponents(
    Registry &reg,
    entity_t entity,
    const nlohmann::json &componentsJson,
    bool /*isShared*/,
    int x,
    int y,
    float vx,
    float vy
)
{
    addCommonComponents(reg, entity, componentsJson, x, y, vx, vy);

    if (componentsJson.contains("drawable")) {
        auto drawableJson = componentsJson["drawable"];
        ecs::component::Drawable drawableComp;
        auto sizeJson = drawableJson["size"];

        drawableComp.shape.setSize(sf::Vector2f(sizeJson["x"].get<float>(), sizeJson["y"].get<float>()));
        auto fillColorJson = drawableJson["fill_color"];

        drawableComp.shape.setFillColor(
            sf::Color(fillColorJson["r"].get<int>(), fillColorJson["g"].get<int>(), fillColorJson["b"].get<int>())
        );

        reg.addComponent(entity, std::move(drawableComp));
    }
    if (componentsJson.contains("ai_actor")) {
        reg.addComponent(entity, ecs::component::AiActor{});
    }
    if (componentsJson.contains("server_share_movement")) {
        reg.addComponent(entity, ecs::component::ServerShareMovement{});
    }
    if (componentsJson.contains("tag")) {
        component::EntityTag tag{componentsJson["tag"].get<component::EntityTag>()};
        reg.addComponent(entity, component::Tag<component::EntityTag>{std::move(tag)});
    }

    if (componentsJson.contains("animation")) {
        auto animJson = componentsJson["animation"];
        ecs::component::Animation animComp;
        animComp.frameTime = animJson["frame_time"].get<float>();

        for (const auto &[stateName, framesJson] : animJson["frames"].items()) {
            for (auto &frameJson : framesJson) {
                animComp.frames[stateName].emplace_back(
                    frameJson["x"].get<int>(),
                    frameJson["y"].get<int>(),
                    frameJson["width"].get<int>(),
                    frameJson["height"].get<int>()
                );
            }
        }
        if (animJson.contains("current_frame")) {
            animComp.currentFrame = animJson["current_frame"].get<size_t>();
        }
        if (animJson.contains("state")) {
            animComp.state = animJson["state"];
        } else {
            animComp.state = "idle";
        }

        reg.addComponent(entity, std::move(animComp));
    }
    if (componentsJson.contains("dobkeratops")) {
        reg.addComponent<ecs::component::DobkeratopsState>(entity, ecs::component::DobkeratopsState{});
    }
}

void ServerEntityFactory::handleNetworkSync(
    Registry & /* reg */,
    entity_t /* entity */,
    const nlohmann::json & /* entityJson */,
    bool /* isShared */
)
{
    // Server does not handle UDP network synchronization
}

} // namespace ecs
