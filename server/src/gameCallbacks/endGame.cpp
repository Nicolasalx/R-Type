/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** endGame
*/

#include "endGame.hpp"
#include "RTypeUDPProtol.hpp"

void rts::endGame(std::list<std::vector<char>> &datasToSend)
{
    datasToSend.push_back(rt::UDPPacket<rt::UDPBody::END_GAME>(rt::UDPCommand::END_GAME, true).serialize());
}
