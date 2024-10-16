/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** parse_arg
*/

#include "ArgParser.hpp"
#include "RTypeServer.hpp"
#include "RoomManager.hpp"

int rts::parse_arg(int argc, const char **argv, rts::RoomManager &roomManager)
{
    eng::ArgParser argParser;

    argParser.addArgument("debug", "d", eng::ArgParser::ArgType::BOOL, false, "Enable debug mode.");
    argParser.addArgument("help", "h", eng::ArgParser::ArgType::BOOL, false, "Print this help message");
    if (!argParser.parse(argc, argv)) {
        argParser.printHelp();
        return 84;
    }
    if (argParser.getValue<bool>("help")) {
        argParser.printHelp();
        return -1;
    }
    if (argParser.getValue<bool>("debug")) {
        roomManager.enableDebugMode();
    }
    return 0;
}
