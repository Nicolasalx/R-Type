function(configure_particles)
    CPMAddPackage(
      NAME sfml_particules
      GITHUB_REPOSITORY thibaudcathala/SFML-Particles
      GIT_TAG "master"
    )
    target_compile_definitions(Particles-s PUBLIC)
endfunction()