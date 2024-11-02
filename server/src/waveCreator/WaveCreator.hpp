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
#include "TimeoutHandler.hpp"
#include "WaveManager.hpp"
#include "entity.hpp"
#include "udp/UDPServer.hpp"
#include <unordered_map>

namespace rts {

class WaveCreator {
    using ai_function_t = std::function<void(ecs::Registry &, entity_t)>;
    using setup_mob_function_t = std::function<void(
        std::list<std::vector<char>> &,
        ntw::UDPServer &,
        ntw::TimeoutHandler &,
        ecs::Registry &,
        entity_t,
        size_t,
        float,
        float,
        float,
        float
    )>;

    public:
    WaveCreator(const std::string &basePath = "./assets/stages");

    WaveCreator(std::string &&basePath = "./assets/stages");

    ~WaveCreator() = default;

    void createStage(
        size_t stage,
        int missileSpawnRate,
        ecs::WaveManager &waveManager,
        std::list<std::vector<char>> &datasToSend,
        ntw::UDPServer &udpServer,
        ntw::TimeoutHandler &timeoutHandler
    );

    private:
    void _initMobsFunctions()
    {
        _setupMobFunc["bydosWave"] = &setupBydosDatas;
        _setupMobFunc["robot"] = &setupRobotDatas;
        _setupMobFunc["dobkeratops"] = &setupDobkeratopsDatas;
        _setupMobFunc["blob"] = &setupBlobDatas;

        _mobAiCreateFunc["bydosWave"] = &getBydosWaveAi;
        _mobAiCreateFunc["robot"] = &getRobotAi;
        _mobAiCreateFunc["dobkeratops"] = &getDobkeratopsAi;
        _mobAiCreateFunc["blob"] = &getBlobAi;
    }

    void _addJSONMobs(
        size_t waveId,
        const nlohmann::json &mobs,
        int missileSpawnRate,
        ecs::WaveManager &waveManager,
        std::list<std::vector<char>> &datasToSend,
        ntw::UDPServer &udpServer,
        ntw::TimeoutHandler &timeoutHandler
    );

    static void setupDobkeratopsDatas(
        std::list<std::vector<char>> &datasToSend,
        ntw::UDPServer &udpServer,
        ntw::TimeoutHandler &timeoutHandler,
        ecs::Registry &reg,
        entity_t e,
        size_t sharedId,
        float x,
        float y,
        float vx,
        float vy
    );
    static void setupRobotDatas(
        std::list<std::vector<char>> &datasToSend,
        ntw::UDPServer &udpServer,
        ntw::TimeoutHandler &timeoutHandler,
        ecs::Registry &reg,
        entity_t e,
        size_t sharedId,
        float x,
        float y,
        float vx,
        float vy
    );
    static void setupBydosDatas(
        std::list<std::vector<char>> &datasToSend,
        ntw::UDPServer &udpServer,
        ntw::TimeoutHandler &timeoutHandler,
        ecs::Registry &reg,
        entity_t e,
        size_t sharedId,
        float x,
        float y,
        float vx,
        float vy
    );
    static void setupBlobDatas(
        std::list<std::vector<char>> &datasToSend,
        ntw::UDPServer &udpServer,
        ntw::TimeoutHandler &timeoutHandler,
        ecs::Registry &reg,
        entity_t e,
        size_t sharedId,
        float x,
        float y,
        float vx,
        float vy
    );

    static std::function<void(ecs::Registry &reg, entity_t e)> getDobkeratopsAi(
        std::list<std::vector<char>> &datasToSend,
        ecs::WaveManager &waveManager,
        int missileSpawnRate,
        float x,
        float y
    );
    static std::function<void(ecs::Registry &reg, entity_t e)> getRobotAi(
        std::list<std::vector<char>> &datasToSend,
        ecs::WaveManager &waveManager,
        int missileSpawnRate,
        float x,
        float y
    );
    static std::function<void(ecs::Registry &reg, entity_t e)> getBydosWaveAi(
        std::list<std::vector<char>> &datasToSend,
        ecs::WaveManager &waveManager,
        int missileSpawnRate,
        float x,
        float y
    );
    static std::function<void(ecs::Registry &reg, entity_t e)> getBlobAi(
        std::list<std::vector<char>> &datasToSend,
        ecs::WaveManager &waveManager,
        int missileSpawnRate,
        float x,
        float y
    );

    std::unordered_map<std::string, setup_mob_function_t> _setupMobFunc;
    std::unordered_map<
        std::string,
        std::function<ai_function_t(std::list<std::vector<char>> &, ecs::WaveManager &, int, float, float)>>
        _mobAiCreateFunc;
    std::string _basePath;
};

} // namespace rts
