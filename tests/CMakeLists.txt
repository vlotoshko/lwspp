set(TESTS_TARGET_SRC_FILES
    framework/catch2/catch.hpp
    framework/fakeit.hpp
    main.cpp
#    MockedPtr.hpp

#    TestHelloBinance.cpp
    TestHelloWorld.cpp
    TestSimpleFeatures.cpp
#    TestSslFeature.cpp
)

file(COPY ${PROJECT_SOURCE_DIR}/tests/TestData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}-tests ${TESTS_TARGET_SRC_FILES})

target_include_directories(${PROJECT_NAME}-tests PRIVATE
    ${PROJECT_SOURCE_DIR}/tests/framework
    ${PROJECT_SOURCE_DIR}/client/include
    ${PROJECT_SOURCE_DIR}/server/include
)

target_link_libraries(${PROJECT_NAME}-tests
    easywebsocket-client
    easywebsocket-server
)

set_target_properties(${PROJECT_NAME}-tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)
