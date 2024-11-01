function(configure_sfml)
    set(SFML_GENERATE_PACKAGE FALSE CACHE BOOL "Generate SFML package" FORCE)
    set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "Use static standard libraries" FORCE)

    CPMAddPackage(
        NAME SFML
        GITHUB_REPOSITORY SFML/SFML
        GIT_TAG 2.6.1
        OPTIONS
            "SFML_GENERATE_PACKAGE=FALSE"
    )
endfunction()