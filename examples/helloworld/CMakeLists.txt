cmake_minimum_required(VERSION 3.20)

if("${PROJECT_NAME}" STREQUAL "")
    set(PROJECT_NAME lwspp)
    set(STANDALONE_PROJECT TRUE)
endif()


project(
    ${PROJECT_NAME}-helloworld
    VERSION 0.0.0
    LANGUAGES C CXX
)

if(NOT COMMAND lwspp_apply_target_defaults)
    include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/lwspp-targets.cmake)
endif()

set(${PROJECT_NAME}_SRC_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILES})

if(STANDALONE_PROJECT)
    set(WebsocketClient_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/../../client/include
    )

    set(WebsocketServer_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/../../server/include
    )

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${WebsocketClient_INCLUDE_DIR}
        ${WebsocketServer_INCLUDE_DIR}
    )
endif()

if(STANDALONE_PROJECT)
    set(TARGET_LWSPP_CLIENT lwspp-client)
    set(TARGET_LWSPP_SERVER lwspp-server)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${TARGET_LWSPP_CLIENT}
    PRIVATE ${TARGET_LWSPP_SERVER}
    PRIVATE ${LWSPP_WEBSOCKETS_TARGET}
    PRIVATE lwspp_common_includes
)

lwspp_apply_target_defaults(${PROJECT_NAME})

if(NOT "${LWSPP_EXAMPLES_INSTALL_DIR}" STREQUAL "")
    if(NOT ${LWSPP_LIBWEBSOCKETS_ROOT} STREQUAL "")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            INSTALL_RPATH "${LWSPP_LIBWEBSOCKETS_ROOT}/lib"
        )
    endif()

    add_custom_target(install-${PROJECT_NAME} DEPENDS ${PROJECT_NAME})

    add_custom_command(
        TARGET install-${PROJECT_NAME}
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}"
        --install "${CMAKE_BINARY_DIR}"
        --component ${PROJECT_NAME}
        --verbose
    )

    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${LWSPP_EXAMPLES_INSTALL_DIR}
        COMPONENT ${PROJECT_NAME}
    )
endif()
