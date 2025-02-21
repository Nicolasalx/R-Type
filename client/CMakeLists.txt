add_executable(r-type_client
    src/main.cpp
    src/register_ecs.cpp
    src/setup_tcp_connection.cpp
    src/setup_udp_connection.cpp
    src/launch_game.cpp
    src/RoomManager.cpp
    src/register_tcp_response.cpp
    src/register_udp_response.cpp
    src/run_gui.cpp
    src/GameOptions.cpp
    src/menu/room_selection.cpp
    src/menu/title_menu.cpp
    src/menu/chat.cpp
    src/menu/inside_room.cpp
    src/menu/accessibility.cpp
    src/menu/option.cpp
    src/menu/scoreBoard.cpp
)

if(ENABLE_CLANG_TIDY STREQUAL "ON" AND CLANG_TIDY)
    set_target_properties(r-type_client PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color"
    )
elseif(ENABLE_CLANG_TIDY STREQUAL "FULL" AND CLANG_TIDY)
    set_target_properties(r-type_client PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color;--header-filter=${CMAKE_SOURCE_DIR}/server/.*\\.hpp$|${CMAKE_SOURCE_DIR}/include/.*\\.hpp$"
    )
endif()

target_compile_options(r-type_client PRIVATE -Wall -Wextra)

target_link_libraries(r-type_client PRIVATE RTYPE-ECS NETWORK UTILS ImGui-SFML::ImGui-SFML Candle-s Particles-s)

target_include_directories(r-type_client PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(r-type_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
