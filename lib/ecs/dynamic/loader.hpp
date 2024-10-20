/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** loader
*/

#pragma once

#include <memory>

#ifdef _WIN32

// namespace ecs {

// template <typename T>
// std::shared_ptr<ecs::DLLoader<T>> getLoader()
// {
//     return std::make_shared<ecs::DLLoader<T>>(WindowsLoader<T>());
// }

// template <typename T>
// std::shared_ptr<ecs::DLLoader<T>> getLoader(const std::string &libPath)
// {
//     return std::make_shared<ecs::DLLoader<T>>(WindowsLoader<T>(libPath));
// }

// } // namespace ecs

#else

    #include "dynamic/UnixDLLoader.hpp"

namespace ecs {

template <typename T>
std::shared_ptr<ecs::DLLoader<T>> getLoader()
{
    return std::make_shared<ecs::DLLoader<T>>(UnixDLLoader<T>());
}

template <typename T>
std::shared_ptr<ecs::DLLoader<T>> getLoader(const std::string &libPath)
{
    return std::make_shared<ecs::DLLoader<T>>(UnixDLLoader<T>(libPath));
}

} // namespace ecs

#endif
