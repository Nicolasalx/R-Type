function(configure_asio)
    set(ASIO_TAG "asio-1-31-0")
    set(ASIO_NO_DEPRECATED ON)

    CPMAddPackage(
        NAME asiocmake
        GIT_REPOSITORY "https://github.com/OlivierLDff/asio.cmake"
        GIT_TAG "main"
    )
endfunction()