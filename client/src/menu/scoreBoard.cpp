/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** scoreBoard
*/

#include "../ScoreBoard.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "../RTypeClient.hpp"
#include "imgui.h"

// ! When player end the game call this function
// rtc::addScore("assets/score/scoreBoard.json", playerName, score);

void writeEmptyJson(const std::string &filePath)
{
    nlohmann::json emptyJson;
    emptyJson["scoreboard"] = nlohmann::json::array();

    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << emptyJson.dump(4);
        outFile.close();
    }
}

void sortScoreboard(nlohmann::json &j)
{
    std::vector<rtc::ScoreEntry> scoreboard;
    for (const auto &entry : j["scoreboard"]) {
        scoreboard.push_back({entry["nom"], entry["score"]});
    }
    std::sort(scoreboard.begin(), scoreboard.end(), [](const rtc::ScoreEntry &a, const rtc::ScoreEntry &b) {
        return a.score > b.score;
    });
    for (size_t i = 0; i < scoreboard.size(); ++i) {
        j["scoreboard"][i] = {{"nom", scoreboard[i].name}, {"score", scoreboard[i].score}};
    }
}

void limitScoreboard(nlohmann::json &j)
{
    std::size_t nbElement = 10;

    if (j.contains("scoreboard") && j["scoreboard"].is_array()) {
        auto &scoreboard = j["scoreboard"];
        if (scoreboard.size() > nbElement) {
            nlohmann::json newScoreboard = nlohmann::json::array();
            for (size_t i = 0; i < nbElement; ++i) {
                newScoreboard.push_back(scoreboard[i]);
            }
            scoreboard = newScoreboard;
        }
    }
}

void rtc::addScore(const std::string &filename, const std::string &playerName, int score)
{
    nlohmann::json j;
    std::ifstream file(filename);

    if (file.is_open()) {
        try {
            file >> j;
        } catch (const nlohmann::json::parse_error &) {
            writeEmptyJson(filename);
            j["scoreboard"] = nlohmann::json::array();
        }
    } else {
        j["scoreboard"] = nlohmann::json::array();
    }
    j["scoreboard"].push_back({{"nom", playerName}, {"score", score}});
    sortScoreboard(j);
    limitScoreboard(j);
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << j.dump(4);
    } else {
        throw std::runtime_error("Unable to open file for writing: " + filename);
    }
}

void loadScoreboard(const std::string &filename, std::vector<rtc::ScoreEntry> &scoreboard)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    nlohmann::json j;
    file >> j;
    sortScoreboard(j);
    for (const auto &entry : j["scoreboard"]) {
        rtc::ScoreEntry scoreEntry;
        scoreEntry.name = entry["nom"];
        scoreEntry.score = entry["score"];
        scoreboard.push_back(scoreEntry);
    }
}

void rtc::renderScoreBoard(const sf::Vector2u &windowSize, bool scoreBoardEnable, const sf::Vector2u &pos)
{
    std::vector<rtc::ScoreEntry> scoreboard;

    float scoreWidth = windowSize.x * 0.3;
    float scoreHeight = windowSize.y * 0.3;
    float scorePosX = pos.x;
    float scorePosY = pos.y;

    if (!scoreBoardEnable) {
        return;
    }

    try {
        loadScoreboard("assets/score/scoreBoard.json", scoreboard);
    } catch (const std::exception &e) {
        writeEmptyJson("assets/score/scoreBoard.json");
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(scorePosX, scorePosY));
    ImGui::SetNextWindowSize(ImVec2(scoreWidth, scoreHeight));
    ImGui::Begin(
        "Score Board", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
    );

    ImGui::Columns(2, "ScoreboardColumns", false);
    ImGui::Separator();
    ImGui::Text("Name");
    ImGui::NextColumn();
    ImGui::Text("Score");
    ImGui::NextColumn();
    ImGui::Separator();

    for (const auto &entry : scoreboard) {
        ImGui::Text("%s", entry.name.c_str());
        ImGui::NextColumn();
        ImGui::Text("%d", entry.score);
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
    ImGui::End();
}
