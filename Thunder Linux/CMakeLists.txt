cmake_minimum_required(VERSION 3.16)

project(ThunderSDK VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt Auto-processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Optimization Flags for Linux Kernel Efficiency
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Optimization Flags for Linux Kernel Efficiency
    add_compile_options(-O3 -march=native -flto -fno-plt -mavx2 -msse4.2 -mfma -maes -mpclmul -D_FORTIFY_SOURCE=2)
    add_link_options(-flto -Wl,-O1 -Wl,--as-needed -Wl,--sort-common)
endif()

# Enable Interprocedural Optimization (LTO)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# Find Qt 6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets)

add_definitions(-DTHUNDER_SHAREDLIBRARY)

include_directories(
    src/lib
    src/lib/app
    src/lib/adblock
    src/lib/kernel
    src/lib/network
)

set(LIB_SRCS
    src/lib/app/thundercommon.cpp
    src/lib/app/mainapplication.cpp
    src/lib/app/browserwindow.cpp
    src/lib/adblock/adblockrule.cpp
    src/lib/kernel/kernel_bridge.cpp
    src/lib/network/thunder_url_interceptor.cpp
)

add_library(ThunderSDK SHARED ${LIB_SRCS})
target_link_libraries(ThunderSDK PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineWidgets
)

# Installation Rules for Library Usage
install(TARGETS ThunderSDK
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install Headers for external software integration
install(DIRECTORY src/lib/ DESTINATION include/thunder
    FILES_MATCHING PATTERN "*.h"
)

# Browser Executable (Now uses the SDK)
add_executable(ThunderBrowser src/main/main.cpp)
target_link_libraries(ThunderBrowser PRIVATE ThunderSDK Qt6::Widgets)

# Generic High-Performance App Example
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/generic_app.cpp")
    add_executable(ThunderGenericExample src/examples/generic_app.cpp)
    target_link_libraries(ThunderGenericExample PRIVATE ThunderSDK Qt6::Core)
endif()

# Baseline App Example (No Thunder)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/no_thunder_app.cpp")
    add_executable(NoThunderBaseline src/examples/no_thunder_app.cpp)
endif()

# Detailed Reports
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/with_thunder_report.cpp")
    add_executable(With_Thunder src/examples/with_thunder_report.cpp)
    target_link_libraries(With_Thunder PRIVATE ThunderSDK Qt6::Core)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/no_thunder_report.cpp")
    add_executable(No_Thunder src/examples/no_thunder_report.cpp)
endif()

# Ultimate Benchmark Suite
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/thunder_ultimate_benchmark.cpp")
    add_executable(Thunder_Ultimate_Benchmark src/examples/thunder_ultimate_benchmark.cpp)
    target_link_libraries(Thunder_Ultimate_Benchmark PRIVATE ThunderSDK Qt6::Core)
endif()

# Ultimate Hardware Stress Test
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/ultimate_stress_test.cpp")
    add_executable(Thunder_Hardware_Stress src/examples/ultimate_stress_test.cpp)
    target_link_libraries(Thunder_Hardware_Stress PRIVATE ThunderSDK Qt6::Core)
endif()

install(TARGETS ThunderBrowser ThunderGenericExample NoThunderBaseline With_Thunder No_Thunder Thunder_Ultimate_Benchmark Thunder_Hardware_Stress RUNTIME DESTINATION bin)
