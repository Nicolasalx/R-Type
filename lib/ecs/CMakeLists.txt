add_library(ECS STATIC
    shared_entity.cpp
)

if(ENABLE_CLANG_TIDY STREQUAL "ON" AND CLANG_TIDY)
    set_target_properties(ECS PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color"
    )
elseif(ENABLE_CLANG_TIDY STREQUAL "FULL" AND CLANG_TIDY)
    set_target_properties(ECS PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color;--header-filter=${CMAKE_SOURCE_DIR}/lib/ecs/.*\\.hpp$"
    )
endif()

target_include_directories(ECS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)
