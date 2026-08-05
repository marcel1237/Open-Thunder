cmake_minimum_required(VERSION 3.16)

project(ThunderSDK VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization Flags for High-Performance SDK
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-O3 -march=native -flto -fno-plt -mavx2 -msse4.2 -mfma -maes -mpclmul -D_FORTIFY_SOURCE=2 -fPIC)
    add_link_options(-flto -Wl,-O1 -Wl,--as-needed -Wl,--sort-common)
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

add_definitions(-DTHUNDER_SHAREDLIBRARY)

include_directories(
    src
    src/app
    src/adblock
    src/kernel
    src/network
)

set(SDK_SRCS
    src/app/thundercommon.cpp
    src/app/mainapplication.cpp
    src/app/browserwindow.cpp
    src/adblock/adblockrule.cpp
    src/kernel/kernel_bridge.cpp
    src/network/thunder_url_interceptor.cpp
)

add_library(ThunderSDK SHARED ${SDK_SRCS})
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
install(TARGETS ThunderSDK
    LIBRARY DESTINATION .
    RUNTIME DESTINATION .
)

install(DIRECTORY src/ DESTINATION include/thunder
    FILES_MATCHING PATTERN "*.h"
)
