/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include <exception>
#include <memory>
#include "ArgParser.hpp"
#include "GameManager.hpp"
#include "InputManager.hpp"
#include "Logger.hpp"
#include "RTypeClient.hpp"
#include "Registry.hpp"
#include <imgui-SFML.h>

void rtc::runGameLoop(
    ecs::Registry &reg,
    const std::shared_ptr<sf::RenderWindow> &window,
    float &dt,
    ecs::InputManager &input,
    std::atomic<GameState> &gameState
)
{
    sf::Clock clock;

    while (window->isOpen() && gameState.load() == GameState::GAME) {
        dt = clock.restart().asSeconds();

        sf::Event event{};
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                gameState.store(GameState::NONE);
            }
            input.update(event);
        }
        window->clear();
        ImGui::SFML::Update(*window, clock.restart());
        reg.runSystems();
        ImGui::SFML::Render(*window);
        window->display();
    }
}

int main(int argc, const char *argv[])
{
    eng::ArgParser argParser;
    argParser.addArgument("ip", "i", eng::ArgParser::ArgType::STRING, true, "Server IP address");
    argParser.addArgument("port", "p", eng::ArgParser::ArgType::INT, true, "Server port");
    argParser.addArgument("player_name", "pn", eng::ArgParser::ArgType::STRING, true, "Player name");
    argParser.addArgument("help", "h", eng::ArgParser::ArgType::BOOL, false, "Print this help message");

    if (!argParser.parse(argc, argv)) {
        argParser.printHelp();
        return 84;
    }
    if (argParser.getValue<bool>("help")) {
        argParser.printHelp();
        return 0;
    }

    auto ip = argParser.getValue<std::string>("ip");
    auto port = argParser.getValue<int>("port");
    auto playerName = argParser.getValue<std::string>("player_name");

    try {
        rtc::GameManager game(ip, port, playerName);
        game.run();
    } catch (const std::exception &e) {
        eng::logError(e.what());
    }
    return 0;
}
