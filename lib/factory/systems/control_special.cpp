/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_special
*/

#include "systems/control_special.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/beam.hpp"
#include "components/controllable.hpp"
#include "components/position.hpp"
#include "udp/UDPClient.hpp"
#include "shared_entity.hpp"

static void spawnMissile(ntw::UDPClient &udp, ecs::component::Position playerPos, int chargeLevel)
{
    rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> msg(
        rt::UDPCommand::NEW_ENTITY_MISSILE, ecs::generateSharedEntityId()
    );
    msg.body.pos = {playerPos.x - 16, playerPos.y + 8};
    msg.body.vel = {250.0f + static_cast<float>(chargeLevel) / 2.0f, 0};
    msg.body.chargeLevel = chargeLevel;
    udp.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void ecs::systems::controlSpecial(ecs::Registry &reg, ntw::UDPClient &udp, int spawnRate)
{
    auto &controllables = reg.getComponents<ecs::component::Controllable>();
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &beams = reg.getComponents<ecs::component::Beam>();

    ecs::Zipper<ecs::component::Controllable, ecs::component::Position, ecs::component::Beam> zipControl(
        controllables, positions, beams
    );

    static auto lastTime = std::chrono::high_resolution_clock::now();
    for (auto [_, pos, beam] : zipControl) {
        if (!beam.isCharging && beam.sendMissile) {
            auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count() < spawnRate) {
                continue;
            }
            lastTime = now;
            spawnMissile(udp, pos, beam.chargeValue);
            beam.chargeValue = 0;
            beam.sendMissile = false;
        }
    }
}
