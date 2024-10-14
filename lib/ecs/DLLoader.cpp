/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DLLoader
*/

#include "DLLoader.hpp"

ecs::DLLoader::DLLoader(const std::string &libPath): _libPath(libPath)
{
    _handle = dlopen(libPath.c_str(), RTLD_LAZY);
    if (!_handle) {
        // throw LibLoadException(dlerror());
    }
}

ecs::DLLoader::~DLLoader()
{
    if (_handle) {
        dlclose(_handle);
    }
}
