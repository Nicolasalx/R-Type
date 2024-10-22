/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClientEntityFactory
*/

#include "ClientEntityFactory.hpp"

#include <utility>
#include "SpriteManager.hpp"
#include "components/animation.hpp"
#include "components/parallax.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "imgui.h"
#include "components/client_share_movement.hpp"
#include "components/death_timer.hpp"
#include "components/music_component.hpp"
#include "components/score_earned.hpp"
#include "components/sound_emitter.hpp"

namespace ecs {

const std::unordered_map<std::string, std::function<void(Registry &, entity_t, ecs::component::Animation &)>>
    ClientEntityFactory::ANIM_MAP = {
        {"player",
         [](ecs::Registry &reg, entity_t id, ecs::component::Animation &anim) {
             auto velOpt = reg.getComponent<ecs::component::Velocity>(id);
             if (!velOpt) {
                 return;
             }

             auto &vel = *velOpt;
             float vy = vel.vy;
             std::string &state = anim.state;

             if (vy > 0) {
                 if (state == "up" || state == "top") {
                     state = "top";
                 } else if (state == "idle") {
                     state = "up";
                 } else {
                     state = "idle";
                 }
             } else if (vy < 0) {
                 if (state == "down" || state == "bottom") {
                     state = "bottom";
                 } else if (state == "idle") {
                     state = "down";
                 } else {
                     state = "idle";
                 }
             } else {
                 if (state == "top") {
                     state = "up";
                 } else if (state == "bottom") {
                     state = "down";
                 } else {
                     state = "idle";
                 }
             }
             std::string &stateSub = reg.getComponent<ecs::component::Sprite>(id)->subSprites[0].animation.state;
             float vx = vel.vx;
             if (vx < 0) {
                 stateSub = "left";
             } else if (vx > 0) {
                 stateSub = "right";
             } else {
                 stateSub = "idle";
             }
         }},
        {"robotGround",
         [](ecs::Registry &reg, entity_t id, ecs::component::Animation &anim) {
             auto velOpt = reg.getComponent<ecs::component::Velocity>(id);
             if (!velOpt) {
                 return;
             }

             float vx = velOpt->vx;
             std::string &state = anim.state;

             if (vx < 0) {
                 state = "left";
             } else {
                 state = "right";
             }
         }},
        {"none", [](ecs::Registry &, entity_t, ecs::component::Animation &) {}},
};

void ClientEntityFactory::addComponents(
    Registry &reg,
    SpriteManager &spriteManager,
    entity_t entity,
    const nlohmann::json &componentsJson,
    bool /*isShared*/,
    int x,
    int y,
    float vx,
    float vy,
    std::shared_ptr<ImFont> font
)
{
    addCommonComponents(reg, entity, componentsJson, x, y, vx, vy, std::move(font));

    if (componentsJson.contains("sprite")) {
        auto spriteJson = componentsJson["sprite"];
        ecs::component::Sprite spriteComp;
        spriteComp.textureId = spriteJson["texture"].get<std::string>();
        spriteComp.spriteObj.setTexture(spriteManager.getTexture(spriteComp.textureId));
        auto frameJson = spriteJson["initial_frame"];
        spriteComp.spriteObj.setTextureRect(sf::IntRect(
            frameJson["x"].get<int>(),
            frameJson["y"].get<int>(),
            frameJson["width"].get<int>(),
            frameJson["height"].get<int>()
        ));
        if (spriteJson.contains("width") && spriteJson.contains("height")) {
            spriteComp.spriteObj.setScale(
                spriteJson["width"].get<float>() / frameJson["width"].get<float>(),
                spriteJson["height"].get<float>() / frameJson["height"].get<float>()
            );
        }
        if (spriteJson.contains("sub_sprites")) {
            for (const auto &subSpriteJson : spriteJson["sub_sprites"]) {
                ecs::component::SubSprite subSprite;
                subSprite.textureId = subSpriteJson["texture"].get<std::string>();
                subSprite.spriteObj.setTexture(spriteManager.getTexture(subSprite.textureId));
                auto subFrameJson = subSpriteJson["initial_frame"];
                subSprite.spriteObj.setTextureRect(sf::IntRect(
                    subFrameJson["x"].get<int>(),
                    subFrameJson["y"].get<int>(),
                    subFrameJson["width"].get<int>(),
                    subFrameJson["height"].get<int>()
                ));
                if (subSpriteJson.contains("x") && subSpriteJson.contains("y")) {
                    subSprite.x = subSpriteJson["x"].get<int>();
                    subSprite.y = subSpriteJson["y"].get<int>();
                }
                if (subSpriteJson.contains("animation")) {
                    auto animJson = subSpriteJson["animation"];
                    subSprite.animation.frameTime = animJson["frame_time"].get<float>();
                    for (const auto &[stateName, framesJson] : animJson["frames"].items()) {
                        for (const auto &frameJson : framesJson) {
                            subSprite.animation.frames[stateName].emplace_back(
                                frameJson["x"].get<int>(),
                                frameJson["y"].get<int>(),
                                frameJson["width"].get<int>(),
                                frameJson["height"].get<int>()
                            );
                        }
                    }
                    if (subSpriteJson.contains("current_frame")) {
                        subSprite.animation.currentFrame = subSpriteJson["current_frame"].get<size_t>();
                    }
                    if (subSpriteJson.contains("state")) {
                        subSprite.animation.state = subSpriteJson["state"].get<std::string>();
                    } else {
                        subSprite.animation.state = "idle";
                    }
                    if (subSpriteJson.contains("update_state")) {
                        subSprite.animation.updateState = ANIM_MAP.at(subSpriteJson["update_state"].get<std::string>());
                    }
                }
                spriteComp.subSprites.push_back(subSprite);
            }
        }
        reg.addComponent(entity, std::move(spriteComp));
    }

    if (componentsJson.contains("parallax")) {
        auto parallaxJson = componentsJson["parallax"];
        ecs::component::Parallax parallaxComp;
        if (parallaxJson.contains("layer")) {
            parallaxComp.layer = parallaxJson["layer"].get<size_t>();
        }
        if (parallaxJson.contains("repeat_x")) {
            parallaxComp.repeatX = parallaxJson["repeat_x"].get<int>();
        }
        if (parallaxJson.contains("repeat_y")) {
            parallaxComp.repeatY = parallaxJson["repeat_y"].get<int>();
        }
        if (parallaxJson.contains("respawn_x")) {
            parallaxComp.respawnX = parallaxJson["respawn_x"].get<int>();
        }
        if (parallaxJson.contains("respawn_y")) {
            parallaxComp.respawnY = parallaxJson["respawn_y"].get<int>();
        }
        reg.addComponent(entity, std::move(parallaxComp));
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
            animComp.state = animJson["state"].get<std::string>();
        } else {
            animComp.state = "idle";
        }
        if (animJson.contains("update_state")) {
            animComp.updateState = ANIM_MAP.at(animJson["update_state"].get<std::string>());
        }

        reg.addComponent(entity, std::move(animComp));
    }
    if (componentsJson.contains("sound_emitter")) {
        auto soundEmitterJson = componentsJson["sound_emitter"];
        ecs::component::SoundEmitter soundEmitterComp;
        soundEmitterComp.soundBufferId = soundEmitterJson["sound_buffer"].get<std::string>();
        soundEmitterComp.volume = soundEmitterJson["volume"].get<float>();
        soundEmitterComp.loop = soundEmitterJson["loop"].get<bool>();
        reg.addComponent(entity, std::move(soundEmitterComp));
    }
    if (componentsJson.contains("music")) {
        auto musicJson = componentsJson["music"];
        ecs::component::MusicComponent musicComp;
        musicComp.musicFilePath = musicJson["file_path"].get<std::string>();
        musicComp.volume = musicJson["volume"].get<float>();
        musicComp.loop = musicJson["loop"].get<bool>();
        musicComp.isPlaying = musicJson["is_playing"].get<bool>();
        reg.addComponent(entity, std::move(musicComp));
    }
    if (componentsJson.contains("client_share_movement")) {
        reg.addComponent(entity, ecs::component::ClientShareMovement{});
    }
    if (componentsJson.contains("death_timer")) {
        auto deathTimerJson = componentsJson["death_timer"];
        ecs::component::DeathTimer deathTimerComp;
        deathTimerComp.timeToDeath = deathTimerJson["time"].get<float>();
        reg.addComponent(entity, std::move(deathTimerComp));
    }
    if (componentsJson.contains("score_earned")) {
        auto scoreJson = componentsJson["score_earned"];
        reg.addComponent(entity, ecs::component::ScoreEarned{scoreJson["points"].get<int>()});
    }
}

} // namespace ecs
