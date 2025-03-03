/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sprite_system
*/

#include "sprite_system.hpp"
#include "IndexedZipper.hpp"
#include "Logger.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/animation.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"

namespace ecs::systems {

void spriteSystem(Registry &reg, float dt, SpriteManager &spriteManager)
{
    auto &sprites = reg.getComponents<component::Sprite>();
    auto &positions = reg.getComponents<component::Position>();
    auto &animations = reg.getComponents<component::Animation>();

    Zipper<component::Sprite, component::Position> spriteZip(sprites, positions);

    for (auto &&[sprite_comp, pos_comp] : spriteZip) {
        if (sprite_comp.spriteObj.getTexture() == nullptr) {
            sf::Texture &texture = spriteManager.getTexture(sprite_comp.textureId);
            sprite_comp.spriteObj.setTexture(texture);
        }
        for (auto &subSprite : sprite_comp.subSprites) {
            if (subSprite.spriteObj.getTexture() == nullptr) {
                sf::Texture &subTexture = spriteManager.getTexture(subSprite.textureId);
                subSprite.spriteObj.setTexture(subTexture);
            }
            subSprite.spriteObj.setPosition(subSprite.x + pos_comp.x, subSprite.y + pos_comp.y);
        }
        sprite_comp.spriteObj.setPosition(pos_comp.x, pos_comp.y);
    }
    IndexedZipper<component::Sprite, component::Animation> animZip(sprites, animations);

    for (auto &&[id, sprite_comp, anim_comp] : animZip) {
        anim_comp.elapsedTime += dt;
        if (anim_comp.elapsedTime >= anim_comp.frameTime) {
            anim_comp.updateState(reg, id, anim_comp);
            anim_comp.elapsedTime = 0.0f;
            auto size = anim_comp.frames[anim_comp.state].size();
            if (size == 0) {
                eng::logError("No frames for state: " + anim_comp.state);
                continue;
            }
            anim_comp.currentFrame = (anim_comp.currentFrame + 1) % anim_comp.frames[anim_comp.state].size();
            sprite_comp.spriteObj.setTextureRect(anim_comp.frames[anim_comp.state][anim_comp.currentFrame]);
        }
        for (auto &subSprite : sprite_comp.subSprites) {
            if (subSprite.animation.frames.empty()) {
                continue;
            }
            subSprite.animation.elapsedTime += dt;
            if (subSprite.animation.elapsedTime >= subSprite.animation.frameTime) {
                subSprite.animation.updateState(reg, id, subSprite.animation);
                subSprite.animation.elapsedTime = 0.0f;
                auto size = subSprite.animation.frames[subSprite.animation.state].size();
                if (size == 0) {
                    eng::logError("No frames for state: " + subSprite.animation.state);
                    continue;
                }
                subSprite.animation.currentFrame = (subSprite.animation.currentFrame + 1) %
                    subSprite.animation.frames[subSprite.animation.state].size();
                subSprite.spriteObj.setTextureRect(
                    subSprite.animation.frames[subSprite.animation.state][subSprite.animation.currentFrame]
                );
            }
        }
    }
}

} // namespace ecs::systems
