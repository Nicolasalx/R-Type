include(cmake/CPM.cmake)

set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Build shared libraries" FORCE)

include(${CMAKE_CURRENT_LIST_DIR}/deps/openal.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/sfml.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/imgui.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/asio.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/json.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/candle.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/particles.cmake)

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

function(configure_dependencies)
    configure_openal()
    configure_sfml()
    configure_imgui()
    configure_asio()
    configure_json()
    configure_candle()
    configure_particles()
endfunction()
