/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DLLoader
*/

#pragma once

#include <dlfcn.h>
#include <stdexcept>
#include <string>

namespace ecs {
class DLLoader {
    public:
    DLLoader(const std::string &libPath);
    DLLoader();

    ~DLLoader();

    template <typename T>
    T getFunction(const std::string &functionName = "entry")
    {
        dlerror();
        void *function = dlsym(_handle, functionName.c_str());
        const char *error = dlerror();
        if (error != nullptr) {
            throw std::runtime_error(error);
        }
        return reinterpret_cast<T>(function);
    }

    const std::string &getLibPath() const
    {
        return _libPath;
    }

    void close();

    void open(const std::string &filepath);

    protected:
    void *_handle = nullptr;
    std::string _libPath;
};

} // namespace ecs
