cmake_minimum_required(VERSION 3.16)

project(ThunderSDK VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(THUNDER_NATIVE_OPTIMIZATIONS "Build for the current machine (not redistributable)" OFF)
option(THUNDER_ENABLE_SANITIZERS "Enable Address/UndefinedBehavior sanitizers" OFF)

# Optimization Flags for High-Performance SDK
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -D_FORTIFY_SOURCE=2)
    if(THUNDER_NATIVE_OPTIMIZATIONS)
        add_compile_options(-O3 -march=native -mavx2 -msse4.2 -mfma -maes -mpclmul)
    endif()
    if(THUNDER_ENABLE_SANITIZERS)
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

# Enable Interprocedural Optimization (LTO)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# Enable Qt Auto-processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets)

set(SDK_SRCS
    src/app/thundercommon.cpp
    src/app/mainapplication.cpp
    src/app/browserwindow.cpp
    src/app/thundertray.cpp
    src/app/thundermemorymonitor.cpp
    src/app/sdkwindow.cpp
    src/adblock/adblockrule.cpp
    src/adblock/adblockmanager.cpp
    src/kernel/kernel_bridge.cpp
    src/network/thunder_url_interceptor.cpp
    src/app/resources.qrc
)

add_library(ThunderSDK SHARED ${SDK_SRCS})
target_compile_definitions(ThunderSDK PRIVATE THUNDER_SHAREDLIBRARY)
target_include_directories(ThunderSDK PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/app>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/kernel>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/network>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(ThunderSDK PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineWidgets
)

# TH-12: Link with liburing if available
find_library(LIBURING uring)
if(LIBURING)
    target_link_libraries(ThunderSDK PRIVATE ${LIBURING})
endif()

# Installation Rules
install(TARGETS ThunderSDK EXPORT ThunderSDKTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/thunder
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT ThunderSDKTargets
    FILE ThunderSDKTargets.cmake
    NAMESPACE Thunder::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ThunderSDK)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/ThunderSDKConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ThunderSDKConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ThunderSDK)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ThunderSDKConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ThunderSDKConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ThunderSDKConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ThunderSDK)

include(CTest)
if(BUILD_TESTING)
    add_executable(ThunderCoreTests tests/core_tests.cpp)
    target_link_libraries(ThunderCoreTests PRIVATE ThunderSDK Qt6::Core)
    add_test(NAME ThunderCoreTests COMMAND ThunderCoreTests)
endif()

# ⚡ ThunderSDK Command Center (Unified Core Service)
add_executable(ThunderSDKApp src/app/main_sdk_app.cpp)
target_link_libraries(ThunderSDKApp PRIVATE ThunderSDK Qt6::Widgets)
