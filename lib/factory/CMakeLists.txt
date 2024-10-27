add_library(FACTORY STATIC
    systems/collision.cpp
    systems/control_move.cpp
    systems/control_special.cpp
    systems/draw.cpp
    systems/draw_player_beam_bar.cpp
    systems/draw_team_data.cpp
    systems/draw_score.cpp
    systems/draw_player_health_bar.cpp
    systems/draw_fps.cpp
    systems/draw_ping.cpp
    systems/sprite_system.cpp
    systems/position.cpp
    systems/missiles_stop.cpp
    systems/client_share_movement.cpp
    systems/server_share_movement.cpp
    systems/health_local_check.cpp
    systems/health_shared_check.cpp
    systems/health_mob_check.cpp
    systems/parallax.cpp
    systems/send_ping.cpp
    systems/death_timer.cpp
    systems/sound_emitter_system.cpp
    systems/death_timer.cpp
    systems/render_radial_light.cpp
    systems/check_game_ending.cpp
    EntityFactory.cpp
    ServerEntityFactory.cpp
    ClientEntityFactory.cpp
    systems/ai_act.cpp
    WaveManager.cpp
    SoundManager.cpp
)

if(ENABLE_CLANG_TIDY STREQUAL "ON" AND CLANG_TIDY)
    set_target_properties(FACTORY PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color"
    )
elseif(ENABLE_CLANG_TIDY STREQUAL "FULL" AND CLANG_TIDY)
    set_target_properties(FACTORY PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color;--header-filter=${CMAKE_SOURCE_DIR}/lib/factory/.*\\.hpp$"
    )
endif()

target_compile_options(FACTORY PUBLIC -Wall -Wextra)

target_include_directories(FACTORY PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(FACTORY PUBLIC nlohmann_json ECS NETWORK sfml-system sfml-window sfml-graphics sfml-audio ImGui-SFML::ImGui-SFML Candle-s)