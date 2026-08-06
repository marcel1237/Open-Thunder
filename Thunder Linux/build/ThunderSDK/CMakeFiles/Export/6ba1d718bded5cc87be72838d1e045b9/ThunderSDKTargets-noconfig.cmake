#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Thunder::ThunderSDK" for configuration ""
set_property(TARGET Thunder::ThunderSDK APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Thunder::ThunderSDK PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "Qt6::Core;Qt6::Gui;Qt6::Widgets;Qt6::WebEngineWidgets"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libThunderSDK.so"
  IMPORTED_SONAME_NOCONFIG "libThunderSDK.so"
  )

list(APPEND _cmake_import_check_targets Thunder::ThunderSDK )
list(APPEND _cmake_import_check_files_for_Thunder::ThunderSDK "${_IMPORT_PREFIX}/lib/libThunderSDK.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
