/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ServerEntityFactory
*/

#include "ServerEntityFactory.hpp"
#include "components/drawable.hpp"
#include "components/ai_actor.hpp"
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
