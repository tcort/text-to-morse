# CMakeLists.txt
# Copyright 2024  Thomas Cort
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty, provided the copyright notice and
# this notice are preserved. This file is offered as-is, without any warranty.

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# project version MUST be an integer
project(text-to-morse VERSION 1.0.0 DESCRIPTION "converts text into a morse code audio file" LANGUAGES C)

set(PROJECT_AUTHOR "Thomas Cort")
set(PROJECT_COPYRIGHT "Copyright (C) 2024  Thomas Cort")
set(PROJECT_LICENSE "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\\nThis is free software: you are free to change and redistribute it.\\nThere is NO WARRANTY, to the extent permitted by law.")

include(GNUInstallDirs)

configure_file (
    "${PROJECT_SOURCE_DIR}/include/version.h.in"
    "${PROJECT_BINARY_DIR}/include/version.h"
)

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}/include"
)

set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/share/pkgconfig")

find_package(PkgConfig REQUIRED)
pkg_check_modules(FLAC REQUIRED flac)
include_directories(${FLAC_INCLUDE_DIRS})
link_directories(${FLAC_LIBRARY_DIRS})

include(CheckFunctionExists)

if(NOT SIN_FUNCTION_EXISTS AND NOT NEED_LINKING_AGAINST_LIBM)
  CHECK_FUNCTION_EXISTS(sin SIN_FUNCTION_EXISTS)
  if(NOT SIN_FUNCTION_EXISTS)
      unset(SIN_FUNCTION_EXISTS CACHE)
      list(APPEND CMAKE_REQUIRED_LIBRARIES m)
      CHECK_FUNCTION_EXISTS(sin SIN_FUNCTION_EXISTS)
      if(SIN_FUNCTION_EXISTS)
          set(NEED_LINKING_AGAINST_LIBM True CACHE BOOL "" FORCE)
      else()
          message(FATAL_ERROR "Failed making the sin() function available")
      endif()
  endif()
endif()

file(GLOB SRC src/*.c)
add_executable(text-to-morse ${SRC})
target_link_libraries(text-to-morse ${FLAC_LIBRARIES})
if (NEED_LINKING_AGAINST_LIBM)
     target_link_libraries(text-to-morse m)
endif()

install(TARGETS text-to-morse DESTINATION bin)

## Packaging

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR ${PROJECT_AUTHOR})
set(CPACK_GENERATOR "") # platform defaults
set(CPACK_SOURCE_GENERATOR "TGZ;TXZ;ZIP")
set(CPACK_PACKAGE_DIRECTORY ${PROJECT_BINARY_DIR}/package)

set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

set(CPACK_SOURCE_IGNORE_FILES .git/ _CPack_Packages/
    ${CMAKE_BINARY_DIR}/ ${PROJECT_BINARY_DIR}/
    build/ ".*~$"
)

include(CPack)
