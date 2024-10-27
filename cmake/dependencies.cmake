include(cmake/CPM.cmake)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_GENERATE_PACKAGE FALSE CACHE BOOL "" FORCE)
set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)
if(WIN32)
    if(CI)
        set(OPENAL_LIBRARY ${CMAKE_SOURCE_DIR}/openal-soft-install/lib/libOpenAL32.dll.a)
        set(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/openal-soft-install/include/AL)
    else()
        set(OPENAL_LIBRARY /usr/x86_64-w64-mingw32/lib/libOpenAL32.dll.a)
        set(OPENAL_INCLUDE_DIR /usr/x86_64-w64-mingw32/include/AL)
    endif()
endif()


CPMAddPackage(
    NAME SFML
    GITHUB_REPOSITORY SFML/SFML
    GIT_TAG 2.6.1
    OPTIONS "SFML_GENERATE_PACKAGE=FALSE"
)

CPMAddPackage(
    NAME imgui
    GITHUB_REPOSITORY ocornut/imgui
    GIT_TAG v1.89.2
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
message(STATUS "IMGUI_DIR: ${IMGUI_DIR}")

CPMAddPackage(
    NAME ImGui-SFML
    GITHUB_REPOSITORY SFML/imgui-sfml
    GIT_TAG 2.6.x
)

set(ASIO_TAG "asio-1-31-0")
set(ASIO_NO_DEPRECATED ON)
CPMAddPackage(
  NAME asiocmake
  GIT_REPOSITORY "https://github.com/OlivierLDff/asio.cmake"
  GIT_TAG "main"
)

CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  GIT_TAG v3.11.3
)

CPMAddPackage(
  NAME sfml_clandle
  GITHUB_REPOSITORY thibaudcathala/SFML-Candle
  GIT_TAG "master"
)
target_compile_definitions(Candle-s PUBLIC RADIAL_LIGHT_FIX)

CPMAddPackage(
  NAME sfml_particules
  GITHUB_REPOSITORY thibaudcathala/SFML-Particles
  GIT_TAG "master"
)

# CPMAddPackage(
#     NAME sol2
#     GITHUB_REPOSITORY ThePhD/sol2
#     GIT_TAG v3.3.0
# )

# CPMAddPackage(
#     NAME lua_static
#     GITHUB_REPOSITORY walterschell/Lua
#     GIT_TAG v5.4.5
# )
