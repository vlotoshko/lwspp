function(lwspp_apply_target_defaults target)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "lwspp_apply_target_defaults: target '${target}' does not exist")
    endif()

    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        LINKER_LANGUAGE CXX
        POSITION_INDEPENDENT_CODE ON
    )

    target_compile_options(${target} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

    if(APPLE)
        target_compile_options(${target} PRIVATE -std=c++14)
    endif()
endfunction()
