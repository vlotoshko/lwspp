cmake_minimum_required(VERSION 3.9)

if("${PROJECT_NAME}" STREQUAL "")
    set(PROJECT_NAME lwspp)
    set(STANDALONE_PROJECT TRUE)
endif()


project(
    ${PROJECT_NAME}-helloworld
    VERSION 0.0.0
    LANGUAGES C CXX
)

set(${PROJECT_NAME}_SRC_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILES})

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

if(STANDALONE_PROJECT)
    set(TARGET_LWSPP_CLIENT lwspp-client)
    set(TARGET_LWSPP_SERVER lwspp-server)
else()
    if(OPTION_BUILD_STATIC)
        set(TARGET_LWSPP_CLIENT ${TARGET_LWSPP_CLIENT_STATIC})
        set(TARGET_LWSPP_SERVER ${TARGET_LWSPP_SERVER_STATIC})
    else()
        set(TARGET_LWSPP_CLIENT ${TARGET_LWSPP_CLIENT_SHARED})
        set(TARGET_LWSPP_SERVER ${TARGET_LWSPP_SERVER_SHARED})
    endif()
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${TARGET_LWSPP_CLIENT}
    PRIVATE ${TARGET_LWSPP_SERVER}
    PRIVATE websockets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

if(NOT "${INSTALL_EXAMPLES}" STREQUAL "")
    if(NOT ${WEBSOCKETS_CUSTOM_DIR} STREQUAL "")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            INSTALL_RPATH "${WEBSOCKETS_CUSTOM_DIR}/lib"
        )
    endif()

    add_custom_target(install-${PROJECT_NAME} DEPENDS ${PROJECT_NAME})

    add_custom_command(
        TARGET install-${PROJECT_NAME}
        COMMAND "${CMAKE_COMMAND}"
        --install "${CMAKE_BINARY_DIR}"
        --component ${PROJECT_NAME}
        --verbose
    )

    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${INSTALL_EXAMPLES}
        COMPONENT ${PROJECT_NAME}
    )
endif()
