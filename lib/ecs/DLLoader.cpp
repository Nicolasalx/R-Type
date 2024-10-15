/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DLLoader
*/

#include "DLLoader.hpp"

ecs::DLLoader::DLLoader() : _handle(nullptr), _libPath() {}

ecs::DLLoader::DLLoader(const std::string &libPath) : _libPath(libPath)
{
    _handle = dlopen(libPath.c_str(), RTLD_LAZY);
    if (!_handle) {
        throw std::runtime_error(dlerror());
    }
}

ecs::DLLoader::~DLLoader()
{
    if (_handle) {
        dlclose(_handle);
    }
}

void ecs::DLLoader::close()
{
    if (_handle) {
        if (dlclose(_handle)) {
            throw std::runtime_error(dlerror());
        }
    }
}

void ecs::DLLoader::open(const std::string &filepath)
{
    this->close();
    _handle = dlopen(filepath.c_str(), RTLD_LAZY);
    if (!_handle) {
        throw std::runtime_error(dlerror());
    }
    _libPath = filepath;
}
