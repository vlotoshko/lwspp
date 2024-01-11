set(TESTS_TARGET_SRC_FILES
    TestClientBuilder.cpp
)

add_executable(${PROJECT_NAME}-tests ${TESTS_TARGET_SRC_FILES})

target_include_directories(${PROJECT_NAME}-tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

if(OPTION_BUILD_STATIC)
    set(TARGET_LWSPP_CLIENT ${TARGET_LWSPP_CLIENT_STATIC})
else()
    set(TARGET_LWSPP_CLIENT ${TARGET_LWSPP_CLIENT_SHARED})
endif()

find_package(Catch2 REQUIRED)

target_link_libraries(${PROJECT_NAME}-tests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE ${TARGET_LWSPP_CLIENT}
    PRIVATE websockets
)

set_target_properties(${PROJECT_NAME}-tests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

if(NOT "${INSTALL_TESTS}" STREQUAL "")
    if(NOT ${WEBSOCKETS_CUSTOM_DIR} STREQUAL "")
        set_target_properties(${PROJECT_NAME}-tests PROPERTIES
            INSTALL_RPATH "${WEBSOCKETS_CUSTOM_DIR}/lib"
        )
    endif()

    add_custom_target(install-${PROJECT_NAME}-tests DEPENDS ${PROJECT_NAME}-tests)

    add_custom_command(
        TARGET install-${PROJECT_NAME}-tests
        COMMAND "${CMAKE_COMMAND}"
        --install "${CMAKE_BINARY_DIR}"
        --component ${PROJECT_NAME}
        --verbose
    )

    install(TARGETS ${PROJECT_NAME}-tests
        RUNTIME DESTINATION ${INSTALL_TESTS}
        COMPONENT ${PROJECT_NAME}
    )

endif()
