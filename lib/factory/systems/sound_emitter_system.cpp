/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sound_emitter_system
*/

#include "sound_emitter_system.hpp"
#include "IndexedZipper.hpp"
#include "components/sound_emitter.hpp"

namespace ecs::systems {

void soundEmitterSystem(ecs::Registry &reg, ecs::SoundManager &soundManager)
{
    auto &emitters = reg.getComponents<ecs::component::SoundEmitter>();

    auto zipper = ecs::IndexedZipper<ecs::component::SoundEmitter>(emitters);

    for (auto [entity, sound] : zipper) {
        soundManager.loadSoundBuffer(sound.soundBufferId, sound.soundBufferId);
        soundManager.playSoundEffect(sound.soundBufferId, sound.volume);
        reg.removeComponent<ecs::component::SoundEmitter>(entity);
    }
}

} // namespace ecs::systems
