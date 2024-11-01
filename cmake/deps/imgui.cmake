function(configure_imgui)
    CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG v1.89.2
    )
    set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "ImGui directory" FORCE)

    CPMAddPackage(
        NAME ImGui-SFML
        GITHUB_REPOSITORY SFML/imgui-sfml
        GIT_TAG 2.6.x
    )
    if(NOT ImGui-SFML_ADDED)
        message(FATAL_ERROR "Failed to add ImGui-SFML")
    endif()
endfunction()