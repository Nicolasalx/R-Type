/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** UnixDLLoader
*/

#pragma once

#ifdef linux
    #include <dlfcn.h>
    #include <stdexcept>
    #include <string>
    #include "DLLoader.hpp"

namespace ecs {
/**
 * @brief Class in charge of loading shared libraries in UNIX System.
 *        The UnixDLLoader load dynamically any shared library given in parameter as a string.
 *
 * @tparam Type of the function that will be loaded from the shared library.
 *
 * NOTE: This implementation only works in UNIX system as we are using dl library and inherit
 *       from DLLoader interface class.
 */
template <typename T>
class UnixDLLoader : public DLLoader<T> {
    public:
    /**
     * @brief Constructor of the UnixDLLoader which takes a path and load the library.
     *
     * @param libPath Path of the shared library to load
     */
    UnixDLLoader(const std::string &libPath) : DLLoader<T>(libPath), _handle(dlopen(libPath.c_str(), RTLD_LAZY))
    {
        if (!_handle) {
            throw std::runtime_error(dlerror());
        }
    }

    /**
     * @brief Default constructor of the UnixDLLoader, library path can be added later.
     */
    UnixDLLoader() : DLLoader<T>() {}

    /**
     * @brief Default destructor of the DLLoader.
     */
    ~UnixDLLoader() override
    {
        close();
    }

    /**
     * @brief Get the function of name @param functionName inside the current shared library opened.
     *
     * @param functionName Name of the function to look for inside the shared library, the default is entry.
     * @return The function that is loaded from the shared library, that has the type specified
     *         in the template parameter of the class.
     */
    T getFunction(const std::string &functionName = "entry") override
    {
        dlerror();
        void *function = dlsym(_handle, functionName.c_str());
        const char *error = dlerror();
        if (error != nullptr) {
            throw std::runtime_error(error);
        }
        return reinterpret_cast<T>(function);
    }

    /**
     * @brief Open the the shared library specified in the parameter @param filepath.
     *
     * @param filepath Path of the shared library to open.
     */
    void open(const std::string &filepath) override
    {
        close();
        _handle = dlopen(filepath.c_str(), RTLD_LAZY);
        if (!_handle) {
            throw std::runtime_error(dlerror());
        }
        this->_libPath = filepath;
    }

    /**
     * @brief Close an opened shared library, it there was one.
     */
    void close() override
    {
        if (_handle) {
            if (dlclose(_handle)) {
                throw std::runtime_error(dlerror());
            }
        }
    }

    protected:
    void *_handle = nullptr;
};

} // namespace ecs

#else

    #error \
        "UnixDLLoader.hpp is the DLLoader implementation for UNIX System, you should include WindowsLoader.hpp instead"

#endif
