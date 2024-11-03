function(configure_sfml)

    set(SFML_GENERATE_PACKAGE FALSE CACHE BOOL "Generate SFML package" FORCE)

    CPMAddPackage(
        NAME SFML
        GITHUB_REPOSITORY SFML/SFML
        GIT_TAG 2.6.1
        OPTIONS 
            "SFML_GENERATE_PACKAGE=FALSE"
            "SFML_BUILD_AUDIO=TRUE"
            "SFML_BUILD_GRAPHICS=TRUE"
            "SFML_BUILD_WINDOW=TRUE"
            "SFML_BUILD_NETWORK=FALSE"
            "SFML_BUILD_EXAMPLES=FALSE"
            "SFML_BUILD_DOC=FALSE"
            "SFML_INSTALL_PKGCONFIG_FILES=FALSE"
    )

    # Verify that audio was built
    if(NOT TARGET sfml-audio)
        message(FATAL_ERROR "SFML audio module was not built!")
    endif()
endfunction()