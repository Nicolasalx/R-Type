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

/**
 * @brief A class for managing key bindings for actions.
 *
 * The `KeyBind` template class allows for the association of actions
 * (e.g., player commands) with specific keyboard keys. It provides
 * methods to add, remove, and modify key bindings dynamically.
 *
 * @tparam Action The type representing the action (e.g., an enum for game actions).
 * @tparam Key The type representing the key (e.g., `sf::Keyboard::Key`).
 */
template <typename Action, typename Key>
class KeyBind {
    private:
    std::unordered_map<Action, Key> _keyBind; ///< The map storing the action-key pairs.

    public:
    KeyBind() = default; ///< Default constructor.

    /**
     * @brief Constructs a KeyBind with an initial set of key bindings.
     *
     * @param keyBind A map of action-key pairs to initialize the key bindings.
     */
    KeyBind(const std::unordered_map<Action, Key> &keyBind) : _keyBind(keyBind) {}

    /**
     * @brief Adds or updates a key binding for a specified action.
     *
     * @param action The action to bind the key to.
     * @param newKey The key to associate with the action.
     */
    void addKeyBind(const Action &action, Key newKey)
    {
        _keyBind[action] = newKey;
    }

    /**
     * @brief Removes the key binding for a specified action.
     *
     * @param action The action whose key binding should be erased.
     */
    void eraseKeyBind(const Action &action)
    {
        _keyBind.erase(action);
    }

    /**
     * @brief Changes the key associated with a specified action.
     *
     * @param action The action for which the key should be changed.
     * @param newKey The new key to associate with the action.
     */
    void changeActionKey(const Action &action, Key newKey)
    {
        _keyBind.at(action) = newKey;
    }

    /**
     * @brief Retrieves the key associated with a specified action.
     *
     * @param action The action for which the key is requested.
     * @return const Key& Reference to the key associated with the action.
     */
    const Key &getActionKey(Action action) const
    {
        return _keyBind.at(action);
    }
};

} // namespace ecs
