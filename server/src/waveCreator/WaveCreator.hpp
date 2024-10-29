/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** WaveCreator
*/

#pragma once

#include <cstddef>
#include <list>
#include <nlohmann/json.hpp>
#include <string>
#include "WaveManager.hpp"
#include <unordered_map>

namespace rts {

class WaveCreator {
    using ai_function_t = std::function<void(ecs::Registry &reg, entity_t e)>;
    using setup_mob_function_t = std::function<
        void(std::list<std::vector<char>> &datasToSend, size_t sharedId, float x, float y, float vx, float vy)>;

    public:
    WaveCreator(const std::string &basePath = "./assets/stages");

    WaveCreator(std::string &&basePath = "./assets/stages");

    ~WaveCreator() = default;

    void createStage(
        size_t stage,
        int missileSpawnRate,
        ecs::WaveManager &waveManager,
        std::list<std::vector<char>> &datasToSend
    );

    private:
    void _initMobsFunctions()
    {
        _setupMobFunc["bydosWave"] = &setupBydosDatas;
        _setupMobFunc["robot"] = &setupRobotDatas;
        _setupMobFunc["dobkeratops"] = &setupDobkeratopsDatas;

        _mobCreateFunc["bydosWave"] = &getBydosWaveAi;
        _mobCreateFunc["robot"] = &getRobotAi;
        _mobCreateFunc["dobkeratops"] = &getDobkeratopsAi;
    }

    void _addJSONMobs(
        size_t waveId,
        const nlohmann::json &mobs,
        int missileSpawnRate,
        ecs::WaveManager &waveManager,
        std::list<std::vector<char>> &datasToSend
    );

    static void setupDobkeratopsDatas(
        std::list<std::vector<char>> &datasToSend,
        size_t sharedId,
        float x,
        float y,
        float vx,
        float vy
    );
    static void setupRobotDatas(
        std::list<std::vector<char>> &datasToSend,
        size_t sharedId,
        float x,
        float y,
        float vx,
        float vy
    );
    static void setupBydosDatas(
        std::list<std::vector<char>> &datasToSend,
        size_t sharedId,
        float x,
        float y,
        float vx,
        float vy
    );

    static std::function<void(ecs::Registry &reg, entity_t e)> getDobkeratopsAi(
        std::list<std::vector<char>> &datasToSend,
        int missileSpawnRate,
        float x,
        float y
    );
    static std::function<void(ecs::Registry &reg, entity_t e)> getRobotAi(
        std::list<std::vector<char>> &datasToSend,
        int missileSpawnRate,
        float x,
        float y
    );
    static std::function<void(ecs::Registry &reg, entity_t e)> getBydosWaveAi(
        std::list<std::vector<char>> &datasToSend,
        int missileSpawnRate,
        float x,
        float y
    );

    std::unordered_map<std::string, setup_mob_function_t> _setupMobFunc;
    std::unordered_map<
        std::string,
        std::function<ai_function_t(std::list<std::vector<char>> &datasToSend, int missileSpawnRate, float x, float y)>>
        _mobCreateFunc;
    std::string _basePath;
};

} // namespace rts
