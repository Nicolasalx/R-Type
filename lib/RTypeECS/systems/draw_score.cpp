/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_score
*/

#include "draw_score.hpp"
#include <cmath>
#include "Zipper.hpp"
#include "components/score.hpp"
#include "imgui.h"
#include "components/self_player.hpp"

void ecs::systems::drawScore(Registry &reg, const sf::Vector2u &windowSize, int &score)
{
    auto &scoring = reg.getComponents<ecs::component::Score>();
    auto &selfPlayer = reg.getComponents<ecs::component::SelfPlayer>();

    Zipper<ecs::component::Score, ecs::component::SelfPlayer> zip(scoring, selfPlayer);

    for (auto [scr, _] : zip) {
        if (scr.font != nullptr) {
            ImGui::GetBackgroundDrawList()->AddText(
                scr.font.get(),
                (windowSize.y * 0.05),
                ImVec2(windowSize.x * 0.95, windowSize.y * 0.10),
                IM_COL32(255, 255, 255, 255),
                std::to_string(scr.value).c_str()
            );
            score = scr.value;
        }
    }
}
