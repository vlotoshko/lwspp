set(TESTS_TARGET_SRC_FILES
    framework/catch2/catch.hpp
    framework/fakeit.hpp
    main.cpp
    MockedPtr.hpp
    Utils.cpp
    Utils.hpp

    TestDataTransfer.cpp
    TestHelloWorld.cpp
    TestSimpleFeatures.cpp
    TestSslFeature.cpp
)

add_executable(${PROJECT_NAME}-tests ${TESTS_TARGET_SRC_FILES})

target_include_directories(${PROJECT_NAME}-tests PRIVATE
    ${PROJECT_SOURCE_DIR}/tests/framework
    ${PROJECT_SOURCE_DIR}/client/include
    ${PROJECT_SOURCE_DIR}/server/include
)

target_link_libraries(${PROJECT_NAME}-tests
    PRIVATE easywebsockets-client
    PRIVATE easywebsockets-server
    PRIVATE websockets
)

set_target_properties(${PROJECT_NAME}-tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

if(NOT "${INSTALL_TESTS}" STREQUAL "")
    if(NOT ${WEBSOCKETS_DIR} STREQUAL "")
        set_target_properties(${PROJECT_NAME}-tests PROPERTIES
            INSTALL_RPATH "${WEBSOCKETS_DIR}/lib"
        )
    endif()

    add_custom_target(${PROJECT_NAME}-tests_install DEPENDS ${PROJECT_NAME}-tests)

    add_custom_command(
        TARGET ${PROJECT_NAME}-tests_install
        COMMAND "${CMAKE_COMMAND}"
        --install "${CMAKE_BINARY_DIR}"
        --component ${PROJECT_NAME}
        --verbose
    )

    install(TARGETS ${PROJECT_NAME}-tests
        RUNTIME DESTINATION ${INSTALL_TESTS}
        COMPONENT ${PROJECT_NAME}
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/tests/TestData
        DESTINATION ${INSTALL_TESTS}
        COMPONENT ${PROJECT_NAME}
    )
endif()
