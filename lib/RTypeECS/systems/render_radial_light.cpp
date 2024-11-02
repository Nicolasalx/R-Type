/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_radial_ligh
*/

#include "render_radial_light.hpp"
#include <vector>
#include "Candle/LightSource.hpp"
#include "SFML/System/Vector2.hpp"
#include "Zipper.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "components/light_edge.hpp"
#include "components/radial_light.hpp"

static std::vector<candle::Edge> updateEdgePosition(ecs::Registry &reg)
{
    auto &pos = reg.getComponents<ecs::component::Position>();
    auto &hitbox = reg.getComponents<ecs::component::Hitbox>();
    auto &lightEdge = reg.getComponents<ecs::component::LightEdge>();

    ecs::Zipper<ecs::component::Position, ecs::component::Hitbox, ecs::component::LightEdge> zip(
        pos, hitbox, lightEdge
    );
    std::vector<candle::Edge> edgeList;

    for (auto [pos, hitbox, lightEdge] : zip) {
        float x1 = pos.x;
        float y1 = pos.y;
        float x2 = x1 + hitbox.width;
        float y2 = y1 + hitbox.height;

        lightEdge.edge[0] = candle::Edge(sf::Vector2f(x1, y1), sf::Vector2f(x2, y1));
        lightEdge.edge[1] = candle::Edge(sf::Vector2f(x1, y2), sf::Vector2f(x2, y2));
        lightEdge.edge[2] = candle::Edge(sf::Vector2f(x1, y1), sf::Vector2f(x1, y2));
        lightEdge.edge[3] = candle::Edge(sf::Vector2f(x2, y1), sf::Vector2f(x2, y2));
        edgeList.push_back(lightEdge.edge[0]);
        edgeList.push_back(lightEdge.edge[1]);
        edgeList.push_back(lightEdge.edge[2]);
        edgeList.push_back(lightEdge.edge[3]);
    }
    return edgeList;
}

void ecs::systems::renderRadialLight(ecs::Registry &reg, sf::RenderWindow &window)
{
    std::vector<candle::Edge> edgeList = updateEdgePosition(reg);

    auto &pos = reg.getComponents<ecs::component::Position>();
    auto &radialLight = reg.getComponents<ecs::component::RadialLight>();

    ecs::Zipper<ecs::component::Position, ecs::component::RadialLight> zip(pos, radialLight);

    for (auto [pos, radialLight] : zip) {
        radialLight.light.setPosition(sf::Vector2f(pos.x + radialLight.offset.x, pos.y + radialLight.offset.y));
        radialLight.light.castLight(edgeList.begin(), edgeList.end());
        window.draw(radialLight.light);
    }
}
