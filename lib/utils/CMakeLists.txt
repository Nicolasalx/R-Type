add_library(UTILS STATIC
    Logger.cpp
    TrackedException.cpp
    ArgParser.cpp
    FrameRateManager.cpp
    RandomGenerator.cpp
)

if(ENABLE_CLANG_TIDY STREQUAL "ON" AND CLANG_TIDY)
    set_target_properties(UTILS PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color"
    )
elseif(ENABLE_CLANG_TIDY STREQUAL "FULL" AND CLANG_TIDY)
    set_target_properties(UTILS PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color;--header-filter=${CMAKE_SOURCE_DIR}/lib/utils/.*\\.hpp$"
    )
endif()

target_include_directories(UTILS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
