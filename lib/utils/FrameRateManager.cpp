/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** FrameRateManager
*/

#include "FrameRateManager.hpp"
#include <string>
#include <thread>
#include "TrackedException.hpp"

eng::FramerateManager::FramerateManager(double targetFPS)
{
    if (targetFPS <= 0) {
        throw eng::TrackedException("Invalide targeted FPS: " + std::to_string(targetFPS) + ".");
    }
    _targetFrameTime = std::chrono::duration<double>(1.0 / targetFPS);
}

void eng::FramerateManager::setFrameRate(double targetFPS)
{
    if (targetFPS <= 0) {
        throw eng::TrackedException("Invalide targeted FPS: " + std::to_string(targetFPS) + ".");
    }
    _targetFrameTime = std::chrono::duration<double>(1.0 / targetFPS);
}

void eng::FramerateManager::start()
{
    _startTime = std::chrono::steady_clock::now();
}

void eng::FramerateManager::end()
{
    auto remainingTime = _targetFrameTime - (std::chrono::steady_clock::now() - _startTime);

    if (remainingTime.count() > 0) {
        std::this_thread::sleep_for(remainingTime);
    }
}
