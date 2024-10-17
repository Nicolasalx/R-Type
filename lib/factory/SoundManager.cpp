/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SoundManager
*/

#include "SoundManager.hpp"
#include "../utils/Logger.hpp"
#include "../utils/TrackedException.hpp"

namespace ecs {

SoundManager::SoundManager() : _currentMusic(nullptr) {}

SoundManager::~SoundManager()
{
    stopAllSoundEffects();
    if (_currentMusic) {
        _currentMusic->stop();
    }
}

bool SoundManager::loadSoundBuffer(const std::string &id, const std::string &filepath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        eng::logError("Failed to load sound buffer: " + filepath);
        return false;
    }
    _soundBuffers.emplace(id, std::move(buffer));
    return true;
}

const sf::SoundBuffer &SoundManager::getSoundBuffer(const std::string &id) const
{
    auto it = _soundBuffers.find(id);
    if (it != _soundBuffers.end()) {
        return it->second;
    }
    throw eng::TrackedException("SoundBuffer not found: " + id);
}

void SoundManager::playSoundEffect(const std::string &bufferId, float volume, bool loop)
{
    sf::Sound sound;
    try {
        sound.setBuffer(getSoundBuffer(bufferId));
    } catch (const std::exception &e) {
        eng::logError("Error playing sound effect: " + std::string(e.what()));
        return;
    }
    sound.setVolume(volume);
    sound.setLoop(loop);
    sound.play();
    _activeSounds.emplace_back(std::make_unique<sf::Sound>(std::move(sound)));

    _activeSounds.erase(
        std::remove_if(
            _activeSounds.begin(),
            _activeSounds.end(),
            [](const std::unique_ptr<sf::Sound> &s) { return s->getStatus() == sf::Sound::Stopped; }
        ),
        _activeSounds.end()
    );
}

void SoundManager::stopAllSoundEffects()
{
    for (auto &sound : _activeSounds) {
        sound->stop();
    }
    _activeSounds.clear();
}

bool SoundManager::loadMusic(const std::string &id, const std::string &filepath)
{
    auto music = std::make_shared<sf::Music>();
    if (!music->openFromFile(filepath)) {
        eng::logError("Failed to load music: " + filepath);
        return false;
    }
    _musics.emplace(id, std::move(music));
    return true;
}

void SoundManager::playMusic(const std::string &id, float volume, bool loop)
{
    if (_currentMusic) {
        _currentMusic->stop();
    }

    auto it = _musics.find(id);
    if (it != _musics.end()) {
        _currentMusic = it->second;
        _currentMusic->setVolume(volume);
        _currentMusic->setLoop(loop);
        _currentMusic->play();
    } else {
        eng::logError("Music ID not found: " + id);
    }
}

void SoundManager::pauseMusic()
{
    if (_currentMusic && _currentMusic->getStatus() == sf::Music::Playing) {
        _currentMusic->pause();
    }
}

void SoundManager::stopMusic()
{
    if (_currentMusic) {
        _currentMusic->stop();
    }
}

void SoundManager::setMusicVolume(float volume)
{
    if (_currentMusic) {
        _currentMusic->setVolume(volume);
    }
}

} // namespace ecs
