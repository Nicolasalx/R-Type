cmake_minimum_required(VERSION 3.10)
project(R-TYPE LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)
set(ENABLE_CLANG_TIDY "ON" CACHE STRING "Enable clang-tidy (OFF, ON, FULL)")
set_property(CACHE ENABLE_CLANG_TIDY PROPERTY STRINGS OFF ON FULL)

# Force static linking for Windows/MinGW builds
if(WIN32 OR MINGW)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
    set(SFML_USE_STATIC_STD_LIBS TRUE CACHE BOOL "Force static link for stdlib" FORCE)
endif()

include(cmake/options.cmake)
include(cmake/dependencies.cmake)
configure_dependencies()

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)

# CPack Configuration
set(CPACK_PACKAGE_NAME "RType")
set(CPACK_PACKAGE_VENDOR "UnrealEngine")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "RType Game engine")
set(CPACK_PACKAGE_VERSION "0.2.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "RType")
set(CPACK_PACKAGE_CHECKSUM "SHA256")

# System-specific configuration
if(CMAKE_CROSSCOMPILING)
    set(CPACK_SYSTEM_NAME "Windows")
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_SYSTEM_NAME "Linux")
    set(CPACK_GENERATOR "TGZ")
endif()

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")

# Installation configuration
install(DIRECTORY assets DESTINATION .)
install(TARGETS r-type_client r-type_server 
    RUNTIME DESTINATION .
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Generate a default configuration file
configure_file(
    "${CMAKE_SOURCE_DIR}/config.ini.in"
    "${CMAKE_BINARY_DIR}/config.ini"
    @ONLY
)
install(FILES "${CMAKE_BINARY_DIR}/config.ini" DESTINATION .)

include(CPack)