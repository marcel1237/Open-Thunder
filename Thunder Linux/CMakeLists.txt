cmake_minimum_required(VERSION 3.16)

project(ThunderBrowser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization Flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-O3 -march=native -flto -fno-plt -mavx2 -msse4.2 -mfma -maes -mpclmul -D_FORTIFY_SOURCE=2)
    add_link_options(-flto -Wl,-O1 -Wl,--as-needed -Wl,--sort-common)
endif()

# Enable Interprocedural Optimization (LTO)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets)

# Include ThunderSDK Headers from the subproject
include_directories(
    "${CMAKE_SOURCE_DIR}/../ThunderSDK/src"
    "${CMAKE_SOURCE_DIR}/../ThunderSDK/src/app"
    "${CMAKE_SOURCE_DIR}/../ThunderSDK/src/adblock"
    "${CMAKE_SOURCE_DIR}/../ThunderSDK/src/kernel"
    "${CMAKE_SOURCE_DIR}/../ThunderSDK/src/network"
)

# Define the location of the compiled SDK library
set(THUNDERSDK_LIB "${CMAKE_SOURCE_DIR}/../ThunderSDK/build/libThunderSDK.so")

# Browser Executable
add_executable(ThunderBrowser src/main/main.cpp)
target_link_libraries(ThunderBrowser PRIVATE "${THUNDERSDK_LIB}" Qt6::Widgets Qt6::WebEngineWidgets)

# Generic High-Performance App Example (SDK Test)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/generic_app.cpp")
    add_executable(ThunderGenericExample src/examples/generic_app.cpp)
    target_link_libraries(ThunderGenericExample PRIVATE "${THUNDERSDK_LIB}" Qt6::Core)
endif()

# Ultimate Benchmark Suite
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/thunder_ultimate_benchmark.cpp")
    add_executable(Thunder_Ultimate_Benchmark src/examples/thunder_ultimate_benchmark.cpp)
    target_link_libraries(Thunder_Ultimate_Benchmark PRIVATE "${THUNDERSDK_LIB}" Qt6::Core)
endif()

# Ultimate Hardware Stress Test
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/ultimate_stress_test.cpp")
    add_executable(Thunder_Hardware_Stress src/examples/ultimate_stress_test.cpp)
    target_link_libraries(Thunder_Hardware_Stress PRIVATE "${THUNDERSDK_LIB}" Qt6::Core)
endif()

# Detailed Reports
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/with_thunder_report.cpp")
    add_executable(With_Thunder src/examples/with_thunder_report.cpp)
    target_link_libraries(With_Thunder PRIVATE "${THUNDERSDK_LIB}" Qt6::Core)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/no_thunder_report.cpp")
    add_executable(No_Thunder src/examples/no_thunder_report.cpp)
endif()
