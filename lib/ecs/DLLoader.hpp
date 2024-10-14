/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DLLoader
*/

#pragma once

#include <dlfcn.h>
#include <string>

namespace ecs {
class DLLoader {
    public:
    DLLoader(const std::string &libPath);
    ~DLLoader();

    template <typename T>
    T getFunction(const std::string &functionName = "entry")
    {
        dlerror();
        void *function = dlsym(_handle, functionName.c_str());
        const char *error = dlerror();
        if (error != nullptr) {
            // throw FuncLoadException(error);
        }
        return reinterpret_cast<T>(function);
    }

    const std::string &getLibPath() const
    {
        return _libPath;
    }

    protected:
    void *_handle = nullptr;
    std::string _libPath;
};
} // namespace ecs
