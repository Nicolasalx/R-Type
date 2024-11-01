/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_game_ending
*/

#include "systems/draw_game_ending.hpp"
#include "GameManager.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/score.hpp"
#include "components/self_player.hpp"

void ecs::systems::drawGameEnding(
    bool win,
    sf::RenderWindow &window,
    const sf::Vector2u &windowSize,
    const std::shared_ptr<ImFont> &font,
    sf::Sprite &background,
    const std::string &playerName,
    const int &score
)
{
    const std::string playerText = "Hello " + playerName + " !";
    const std::string scoreText = "You have " + std::to_string(score) + " points";
    const std::string resultText = win ? "You Win !" : "You lose !";

    if (background.getTexture()) {
        window.draw(background);
    }

    ImGui::GetBackgroundDrawList()->AddText(
        font.get(),
        (windowSize.y * 0.05),
        ImVec2(0.38f * windowSize.x, 0.1f * windowSize.y),
        IM_COL32(255, 255, 255, 255),
        playerText.c_str()
    );

    ImGui::GetBackgroundDrawList()->AddText(
        font.get(),
        (windowSize.y * 0.05),
        ImVec2(0.35f * windowSize.x, 0.2f * windowSize.y),
        IM_COL32(255, 255, 255, 255),
        scoreText.c_str()
    );

    ImGui::GetBackgroundDrawList()->AddText(
        font.get(),
        (windowSize.y * 0.05),
        ImVec2(0.43f * windowSize.x, 0.3f * windowSize.y),
        IM_COL32(255, 255, 255, 255),
        resultText.c_str()
    );

    rtc::renderScoreBoard(
        windowSize,
        true,
        sf::Vector2u{static_cast<unsigned int>(windowSize.x * 0.35f), static_cast<unsigned int>(windowSize.y * 0.4f)}
    );
}
