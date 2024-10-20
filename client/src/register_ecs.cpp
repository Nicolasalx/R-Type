/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <list>
#include "ClientTickRate.hpp"
#include "Metric.hpp"
#include "RTypeClient.hpp"
#include "RTypeClientConst.hpp"
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
#include "components/music_component.hpp"
#include "components/self_player.hpp"
#include "components/sound_emitter.hpp"
#include "imgui-SFML.h"
#include "systems/client_share_movement.hpp"
#include "systems/control_move.hpp"
#include "systems/control_special.hpp"
#include "systems/draw_fps.hpp"
#include "systems/draw_ping.hpp"
#include "systems/draw_player_beam_bar.hpp"
#include "systems/draw_player_health_bar.hpp"
#include "systems/draw_score.hpp"
#include "systems/draw_team_data.hpp"
#include "systems/health_local_check.hpp"
#include "systems/send_ping.hpp"
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
}

void rtc::registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::UDPClient &udpClient,
    ecs::InputManager &input,
    ntw::TickRateManager<rtc::TickRate> &tickRateManager,
    ecs::SpriteManager &spriteManager,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    std::unordered_map<rtc::ClientMetric, rtc::Metric> &metrics
)
{
    tickRateManager.addTickRate(
        rtc::TickRate::ENTITY_MOVEMENT, rtc::CLIENT_TICKRATE.at(rtc::TickRate::ENTITY_MOVEMENT)
    );
    tickRateManager.addTickRate(rtc::TickRate::PING_REFRESH, rtc::CLIENT_TICKRATE.at(rtc::TickRate::PING_REFRESH));
    tickRateManager.addTickRate(
        rtc::TickRate::CALL_NETWORK_CALLBACKS, rtc::CLIENT_TICKRATE.at(rtc::TickRate::CALL_NETWORK_CALLBACKS)
    );

    reg.addSystem([&reg, &input]() { ecs::systems::controlMove(reg, input); });
    reg.addSystem([&reg, &input, &udpClient]() { ecs::systems::controlSpecial(reg, input, udpClient); });
    reg.addSystem([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.addSystem([&reg]() { ecs::systems::collisionPredict(reg); });
    reg.addSystem([&reg]() { ecs::systems::healthLocalCheck(reg); });
    reg.addSystem([&reg]() { ecs::systems::parallax(reg); });
    reg.addSystem([&reg, &dt, &spriteManager]() { ecs::systems::spriteSystem(reg, dt, spriteManager); });
    reg.addSystem([&reg, &window]() { ecs::systems::draw(reg, window); });
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
    reg.addSystem([&networkCallbacks, &tickRateManager, &dt, &reg]() {
        if (tickRateManager.needUpdate(rtc::TickRate::CALL_NETWORK_CALLBACKS, dt)) {
            while (!networkCallbacks.empty()) {
                networkCallbacks.front()(reg);
                networkCallbacks.pop_front();
            }
        }
    });
    reg.addSystem([&reg, &window]() { ecs::systems::drawPlayerBeamBar(reg, window.getSize()); });
    reg.addSystem([&reg, &window]() { ecs::systems::drawPlayerHealthBar(reg, window.getSize()); });
    reg.addSystem([&reg, &window]() { ecs::systems::drawScore(reg, window.getSize()); });
    reg.addSystem([&reg, &window]() { ecs::systems::drawTeamData(reg, window.getSize()); });
    reg.addSystem([&metrics, &dt, &window]() {
        ecs::systems::drawFPS(metrics.at(rtc::ClientMetric::FPS), dt, window.getSize());
    });
    reg.addSystem([&metrics, &window]() {
        ecs::systems::drawPing(metrics.at(rtc::ClientMetric::PING), window.getSize());
    });
}
