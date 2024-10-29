function(configure_candle)
    CPMAddPackage(
        NAME sfml_clandle
        GITHUB_REPOSITORY thibaudcathala/SFML-Candle
        GIT_TAG "master"
    )
    target_compile_definitions(Candle-s PUBLIC RADIAL_LIGHT_FIX)
endfunction()