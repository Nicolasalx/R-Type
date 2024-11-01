function(configure_lua)
    CPMAddPackage(
        NAME sol2
        GITHUB_REPOSITORY ThePhD/sol2
        GIT_TAG v3.3.0
    )
    CPMAddPackage(
        NAME lua_static
        GITHUB_REPOSITORY walterschell/Lua
        GIT_TAG v5.4.5
    )
endfunction()