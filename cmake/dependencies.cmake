include(cmake/CPM.cmake)

set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Build shared libraries" FORCE)

include(${CMAKE_CURRENT_LIST_DIR}/deps/openal.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/sfml.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/imgui.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/asio.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/json.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/deps/candle.cmake)

function(configure_dependencies)
    configure_openal()
    configure_sfml()
    configure_imgui()
    configure_asio()
    configure_json()
    configure_candle()
endfunction()
