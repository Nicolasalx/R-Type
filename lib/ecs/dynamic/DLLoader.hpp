/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DLLoader
*/

#pragma once

#include <string>

namespace ecs {
/**
 * @brief Interface class for loading shared libraries in any operation system.
 * The DLLoader load dynamically any shared library given the path as parameter in a string.
 *
 * @tparam Type of the function that will be loaded from the shared library.
 *
 * NOTE: As said before this class is an interface, so to use it you MUST implement your own
 *       implementation of the class inheriting from this interface.
 */
template <typename T>
class DLLoader {
    public:
    /**
     * @brief Constructor of the DLLoader which takes a path and load the library.
     *
     * @param libPath Path of the shared library to load
     */
    DLLoader(const std::string &libPath) : _libPath(libPath) {}

    /**
     * @brief Default constructor of the DLLoader, library path can be added later.
     */
    DLLoader() : _libPath() {}

    /**
     * @brief Default destructor of the DLLoader.
     *
     * NOTE: Inside the destructor, the shared library should be closed (if it is openned)
     *       in any operating system.
     */
    virtual ~DLLoader() = default;

    /**
     * @brief Get the function of name @param functionName inside the current shared library opened.
     *
     * @param functionName Name of the function to look for inside the shared library, the default is entry.
     * @return The function that is loaded from the shared library, that has the type specified
     *         in the template parameter of the class.
     */
    virtual T getFunction(const std::string &functionName = "entry") = 0;

    /**
     * @brief Get the path of the current shared library opened.
     *
     * @return The library path as `const std::string &`.
     */
    const std::string &getLibPath() const
    {
        return _libPath;
    }

    /**
     * @brief Open the the shared library specified in the parameter @param filepath.
     *
     * @param filepath Path of the shared library to open.
     *
     * NOTE: In the implementation, if a library is still opened, it must be closed.
     */
    virtual void open(const std::string &filepath) = 0;

    /**
     * @brief Close an opened shared library, it there was one.
     */
    virtual void close() = 0;

    protected:
    std::string _libPath;
};

} // namespace ecs
