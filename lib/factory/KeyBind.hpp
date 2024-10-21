/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** KeyMap
*/

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <sys/stat.h>
#include <unordered_map>

namespace ecs {

template <typename Action, typename Key>
class KeyBind {
    private:
    std::unordered_map<Action, Key> _keyBind;

    public:
    KeyBind() = default;

    KeyBind(const std::unordered_map<Action, Key> &keyBind) : _keyBind(keyBind) {}

    void addKeyBind(const Action &action, Key newKey)
    {
        _keyBind[action] = newKey;
    }

    void eraseKeyBind(const Action &action)
    {
        _keyBind.erase(action);
    }

    void changeActionKey(const Action &action, Key newKey)
    {
        _keyBind.at(action) = newKey;
    }

    const Key &getActionKey(Action action) const
    {
        return _keyBind.at(action);
    }
};

} // namespace ecs
