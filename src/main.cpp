/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-1-rtype-thibaud.cathala
** File description:
** main
*/

#include <iostream>
#include <memory>
#include <stdexcept>
#include "my_log.hpp"
#include "my_tracked_exception.hpp"

int main(int argc, const char *argv[])
{
    try {
        throw my::tracked_exception("R-TYPE !!!!!!!!!!!!!");
    } catch (const std::exception &exception) {
        my::log::error(exception.what());
        return 84;
    } catch (...) {
        my::log::error("Unknow error.");
        return 84;
    }
    return 0;
}
