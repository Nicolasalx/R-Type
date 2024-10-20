/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** WindowsLoader
*/

#pragma once

#ifdef _WIN32
    #include <climits>

    // #include <windows.h> 
    #include <stdexcept>
    #include <string>
    #include "dynamic/DLLoader.hpp"

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
class WindowsLoader : public DLLoader<T> {
    public:
    /**
     * @brief Constructor of the WindowsLoader which takes a path and load the library.
     *
     * @param libPath Path of the shared library to load
     */
    WindowsLoader(const std::string &libPath) :
        DLLoader<T>(libPath)
        // _handle(dlopen(libPath.c_str(), RTLD_LAZY))
    {
        // if (!_handle) {
            // throw std::runtime_error(dlerror());
        // }
    }

    /**
     * @brief Default constructor of the WindowsLoader, library path can be added later.
     */
    WindowsLoader() : DLLoader<T>() {}

    /**
     * @brief Default destructor of the DLLoader.
     */
    ~WindowsLoader() override
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
        // dlerror();
        // void *function = dlsym(_handle, functionName.c_str());
        // const char *error = dlerror();
        // if (error != nullptr) {
            // throw std::runtime_error(error);
        // }
        // return reinterpret_cast<T>(function);
    }

    /**
     * @brief Open the the shared library specified in the parameter @param filepath.
     *
     * @param filepath Path of the shared library to open.
     */
    void open(const std::string &filepath) override
    {
        close();
        // _handle = dlopen(filepath.c_str(), RTLD_LAZY);
        // if (!_handle) {
            // throw std::runtime_error(dlerror());
        // }
        // this->_libPath = filepath;
    }

    /**
     * @brief Close an opened shared library, it there was one.
     */
    void close() override
    {
        // if (_handle) {
            // if (dlclose(_handle)) {
                // throw std::runtime_error(dlerror());
            // }
        // }
    }

    protected:
    // void *_handle = nullptr;
    // HINSTANCE hinstLib;
};

} // namespace ecs

#else

    #error \
        "WindowsLoader.hpp is the DLLoader implementation for Windows, you should include UnixDLLoader.hpp instead"

#endif
