add_executable(r-type_server
    src/parse_arg.cpp
    src/main.cpp
    src/register_ecs.cpp
    src/GameRunner.cpp
    src/register_tcp_response.cpp
    src/register_udp_response.cpp
    src/RoomManager.cpp
    src/init_waves.cpp
    src/command_handler.cpp
    src/ais/waveAi.cpp
    src/ais/fireRandomMissileAi.cpp
    src/ais/horizontalMoveAi.cpp
    src/ais/dobkeratops_ai.cpp
    src/gameCallbacks/collideEffect.cpp
    src/gameCallbacks/endGame.cpp
    src/waveCreator/WaveCreator.cpp
)

if(ENABLE_CLANG_TIDY STREQUAL "ON" AND CLANG_TIDY)
    set_target_properties(r-type_server PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color"
    )
elseif(ENABLE_CLANG_TIDY STREQUAL "FULL" AND CLANG_TIDY)
    set_target_properties(r-type_server PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color;--header-filter=${CMAKE_SOURCE_DIR}/server/.*\\.hpp$"
    )
endif()

target_compile_options(r-type_server PRIVATE -Wall -Wextra)
target_link_libraries(r-type_server PRIVATE FACTORY NETWORK UTILS)
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)
set_target_properties(r-type_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
