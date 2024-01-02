include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(package lwsppServer)

set(CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${package}")

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${package}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${package}Config.cmake
    INSTALL_DESTINATION ${CMAKE_CONFIG_DESTINATION}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${package}ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/lwspp DESTINATION include)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${package}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${package}ConfigVersion.cmake
    DESTINATION ${CMAKE_CONFIG_DESTINATION}
)

install(EXPORT lwsppServerTargets
    NAMESPACE lwspp::
    DESTINATION ${CMAKE_CONFIG_DESTINATION}
)

install(TARGETS ${TARGET_LWSPP_SERVER_STATIC} ${TARGET_LWSPP_SERVER_SHARED}
    EXPORT lwsppServerTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
