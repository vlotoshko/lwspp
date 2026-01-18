set(TESTS_TARGET_SRC_FILES
    TestClientBuilder.cpp
)

add_executable(${PROJECT_NAME}-tests ${TESTS_TARGET_SRC_FILES})

target_include_directories(${PROJECT_NAME}-tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

lwspp_apply_target_defaults(${PROJECT_NAME}-tests)

find_package(Catch2 REQUIRED)

target_link_libraries(${PROJECT_NAME}-tests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE ${TARGET_LWSPP_CLIENT}
    PRIVATE ${LWSPP_WEBSOCKETS_TARGET}
)

if(NOT "${LWSPP_TESTS_INSTALL_DIR}" STREQUAL "")
    if(NOT ${LWSPP_LIBWEBSOCKETS_ROOT} STREQUAL "")
        set_target_properties(${PROJECT_NAME}-tests PROPERTIES
            INSTALL_RPATH "${LWSPP_LIBWEBSOCKETS_ROOT}/lib"
        )
    endif()

    add_custom_target(install-${PROJECT_NAME}-tests DEPENDS ${PROJECT_NAME}-tests)

    add_custom_command(
        TARGET install-${PROJECT_NAME}-tests
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}"
        --install "${CMAKE_BINARY_DIR}"
        --component ${PROJECT_NAME}
        --verbose
    )

    install(TARGETS ${PROJECT_NAME}-tests
        RUNTIME DESTINATION ${LWSPP_TESTS_INSTALL_DIR}
        COMPONENT ${PROJECT_NAME}
    )

endif()
