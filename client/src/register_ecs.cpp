/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include "ClientTickRate.hpp"
#include "GameManager.hpp"
#include "GameOptions.hpp"
#include "MetricManager.hpp"
#include "RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SafeList.hpp"
#include "SoundManager.hpp"
#include "SpriteManager.hpp"
#include "TickRateManager.hpp"
#include "components/animation.hpp"
#include "components/beam.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/health.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/parallax.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/score.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/parallax.hpp"
#include "systems/position.hpp"
#include "components/ally_player.hpp"
#include "components/client_share_movement.hpp"
#include "components/death_timer.hpp"
#include "components/is_a_boss.hpp"
#include "components/light_edge.hpp"
#include "components/music_component.hpp"
#include "components/on_death.hpp"
#include "components/particle_spawner.hpp"
#include "components/radial_light.hpp"
#include "components/score_earned.hpp"
#include "components/self_player.hpp"
#include "components/sound_emitter.hpp"
#include "imgui-SFML.h"
#include "systems/client_share_movement.hpp"
#include "systems/control_move.hpp"
#include "systems/control_special.hpp"
#include "systems/death_timer.hpp"
#include "systems/draw_fps.hpp"
#include "systems/draw_game_ending.hpp"
#include "systems/draw_particle.hpp"
#include "systems/draw_ping.hpp"
#include "systems/draw_player_beam_bar.hpp"
#include "systems/draw_player_health_bar.hpp"
#include "systems/draw_score.hpp"
#include "systems/draw_team_data.hpp"
#include "systems/health_local_check.hpp"
#include "systems/render_radial_light.hpp"
#include "systems/send_ping.hpp"
#include "systems/sound_emitter_system.hpp"
#include "systems/sprite_system.hpp"
#include <unordered_map>

void rtc::registerComponents(ecs::Registry &reg)
{
    reg.registerComponent<ecs::component::Position>();
    reg.registerComponent<ecs::component::Velocity>();
    reg.registerComponent<ecs::component::Drawable>();
    reg.registerComponent<ecs::component::Sprite>();
    reg.registerComponent<ecs::component::Animation>();
    reg.registerComponent<ecs::component::Controllable>();
    reg.registerComponent<ecs::component::Hitbox>();
    reg.registerComponent<ecs::component::ClientShareMovement>();
    reg.registerComponent<ecs::component::SharedEntity>();
    reg.registerComponent<ecs::component::Missile>();
    reg.registerComponent<ecs::component::Parallax>();
    reg.registerComponent<ecs::component::Health>();
    reg.registerComponent<ecs::component::SoundEmitter>();
    reg.registerComponent<ecs::component::MusicComponent>();
    reg.registerComponent<ecs::component::Beam>();
    reg.registerComponent<ecs::component::Score>();
    reg.registerComponent<ecs::component::Player>();
    reg.registerComponent<ecs::component::SelfPlayer>();
    reg.registerComponent<ecs::component::AllyPlayer>();
    reg.registerComponent<ecs::component::ScoreEarned>();
    reg.registerComponent<ecs::component::DeathTimer>();
    reg.registerComponent<ecs::component::OnDeath>();
    reg.registerComponent<ecs::component::RadialLight>();
    reg.registerComponent<ecs::component::LightEdge>();
    reg.registerComponent<ecs::component::ParticleSpawner>();
    reg.registerComponent<ecs::component::IsABoss>();
    reg.registerComponent<ecs::component::ParticleSpawner>();
}

void rtc::registerEndingSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    const std::shared_ptr<ImFont> &font,
    std::atomic<GameState> &gameState,
    const std::string &playerName,
    const int &score
)
{
    bool win = gameState.load() == GameState::WIN;
    sf::Texture texture;
    if (!texture.loadFromFile("assets/menu/background.jpg")) {
        eng::logError("Failed to load background image !");
        return;
    }

    rtc::addScore("assets/score/scoreBoard.json", playerName, score);

    reg.addSystem([win, &window, font, &gameState, texture, &playerName, &score]() {
        sf::Sprite background(texture);
        background.setScale(
            rt::SCREEN_WIDTH / float(texture.getSize().x), rt::SCREEN_HEIGHT / float(texture.getSize().y)
        );
        sf::Color color = background.getColor();
        color.a = 100;
        background.setColor(color);
        ecs::systems::drawGameEnding(win, window, window.getSize(), font, gameState, background, playerName, score);
    });
}

void rtc::registerGameSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::UDPClient &udpClient,
    ecs::InputManager &input,
    ntw::TickRateManager<rtc::TickRate> &tickRateManager,
    ecs::SpriteManager &spriteManager,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ecs::MetricManager<rt::GameMetric> &metrics,
    const ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind,
    ecs::SoundManager &soundManager,
    int &score,
    sf::Clock &chargeClock
)
{
    tickRateManager.addTickRate(
        rtc::TickRate::ENTITY_MOVEMENT, rtc::CLIENT_TICKRATE.at(rtc::TickRate::ENTITY_MOVEMENT)
    );
    tickRateManager.addTickRate(rtc::TickRate::PING_REFRESH, rtc::CLIENT_TICKRATE.at(rtc::TickRate::PING_REFRESH));
    tickRateManager.addTickRate(
        rtc::TickRate::CALL_NETWORK_CALLBACKS, rtc::CLIENT_TICKRATE.at(rtc::TickRate::CALL_NETWORK_CALLBACKS)
    );
    reg.addSystem([&reg, &soundManager]() { ecs::systems::soundEmitterSystem(reg, soundManager); });
    reg.addSystem([&reg, &dt]() { ecs::systems::deathTimer(reg, dt); });
    reg.addSystem([&reg, &input, &keyBind]() { ecs::systems::controlMove(reg, input, keyBind); });
    reg.addSystem([&reg, &udpClient]() {
        ecs::systems::controlSpecial(reg, udpClient, rtc::GameOptions::missileSpawnRate);
    });
    reg.addSystem([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.addSystem([&reg]() { ecs::systems::collisionPredict(reg); });
    reg.addSystem([&reg]() { ecs::systems::healthLocalCheck(reg); });
    reg.addSystem([&reg]() { ecs::systems::parallax(reg); });
    reg.addSystem([&reg, &dt, &spriteManager]() { ecs::systems::spriteSystem(reg, dt, spriteManager); });
    reg.addSystem([&reg, &window]() { ecs::systems::draw(reg, window); });
    reg.addSystem([&reg, &dt, &window]() { ecs::systems::drawParticle(reg, sf::seconds(dt), window); });
    if (rtc::GameOptions::lightEffect) {
        reg.addSystem([&reg, &window]() { ecs::systems::renderRadialLight(reg, window); });
    }
    reg.addSystem([&udpClient, &tickRateManager, &dt]() {
        if (tickRateManager.needUpdate(rtc::TickRate::PING_REFRESH, dt)) {
            ecs::systems::sendPing(udpClient);
        }
    });
    reg.addSystem([&reg, &udpClient, &tickRateManager, &dt]() {
        if (tickRateManager.needUpdate(rtc::TickRate::ENTITY_MOVEMENT, dt)) {
            ecs::systems::clientShareMovement(reg, udpClient);
        }
    });
    reg.addSystem([&networkCallbacks, &tickRateManager, &dt]() {
        if (tickRateManager.needUpdate(rtc::TickRate::CALL_NETWORK_CALLBACKS, dt)) {
            networkCallbacks.consumeList();
        }
    });
    reg.addSystem([&reg, &window, &input, &chargeClock, &keyBind]() {
        ecs::systems::drawPlayerBeamBar(reg, window.getSize(), input, chargeClock, keyBind);
    });
    reg.addSystem([&reg, &window]() { ecs::systems::drawPlayerHealthBar(reg, window.getSize()); });
    reg.addSystem([&reg, &window, &score]() { ecs::systems::drawScore(reg, window.getSize(), score); });
    reg.addSystem([&reg, &window]() { ecs::systems::drawTeamData(reg, window.getSize()); });
    reg.addSystem([&metrics, &dt, &window]() {
        ecs::systems::drawFPS(metrics.getMetric(rt::GameMetric::FPS), dt, window.getSize());
    });
    reg.addSystem([&metrics, &window]() {
        ecs::systems::drawPing(metrics.getMetric(rt::GameMetric::PING), window.getSize());
    });
}
