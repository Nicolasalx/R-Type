add_library(NETWORK STATIC
    tcp/TCPServer.cpp
    tcp/TCPClient.cpp
    udp/UDPServer.cpp
    udp/UDPClient.cpp
)

if(ENABLE_CLANG_TIDY STREQUAL "ON" AND CLANG_TIDY)
    set_target_properties(NETWORK PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color"
    )
elseif(ENABLE_CLANG_TIDY STREQUAL "FULL" AND CLANG_TIDY)
    set_target_properties(NETWORK PROPERTIES CXX_CLANG_TIDY
        "${CLANG_TIDY};--use-color;--header-filter=${CMAKE_SOURCE_DIR}/lib/network/.*\\.hpp$"
    )
endif()

target_compile_options(NETWORK PUBLIC -Wall -Wextra)

target_include_directories(NETWORK PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(NETWORK PUBLIC asio)

if (WIN32)
    target_link_libraries(NETWORK PUBLIC ws2_32 mswsock bcrypt)
endif()
