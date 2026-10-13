cmake_minimum_required(VERSION 3.20)
project(url_beast LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(_vendored_boost "${CMAKE_CURRENT_SOURCE_DIR}/boost_1_92_0")
add_library(boost_headers INTERFACE)

if(BOOST_ROOT)
  if(NOT EXISTS "${BOOST_ROOT}/boost/version.hpp")
    message(FATAL_ERROR "BOOST_ROOT=${BOOST_ROOT} has no boost/version.hpp")
  endif()
  message(STATUS "Boost: using BOOST_ROOT=${BOOST_ROOT}")
  target_include_directories(boost_headers SYSTEM INTERFACE "${BOOST_ROOT}")

elseif(EXISTS "${_vendored_boost}/boost/version.hpp")
  message(STATUS "Boost: using vendored copy at ${_vendored_boost}")
  target_include_directories(boost_headers SYSTEM INTERFACE "${_vendored_boost}")

else()
  find_package(Boost CONFIG QUIET)
  if(Boost_FOUND)
    message(STATUS "Boost: using system package at ${Boost_DIR}")
    target_link_libraries(boost_headers INTERFACE Boost::headers)
  else()
    find_path(BOOST_SYSTEM_INCLUDE_DIR boost/version.hpp)
    if(BOOST_SYSTEM_INCLUDE_DIR)
      message(STATUS "Boost: using headers in ${BOOST_SYSTEM_INCLUDE_DIR}")
      target_include_directories(boost_headers SYSTEM INTERFACE "${BOOST_SYSTEM_INCLUDE_DIR}")
    else()
      message(FATAL_ERROR
        "Boost not found. Tried: -DBOOST_ROOT, ${_vendored_boost}, "
        "system CMake packages, and standard include directories. "
        "Unpack Boost into the repo, install it (e.g. brew install boost), "
        "or pass -DBOOST_ROOT=/path/to/boost")
    endif()
  endif()
endif()

find_package(Threads REQUIRED)

add_executable(url_beast
  src/main.cpp
  src/router.cpp
  src/handlers.cpp
  src/server.cpp
)

target_compile_features(url_beast PRIVATE cxx_std_23)
target_compile_options(url_beast PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(url_beast PRIVATE boost_headers Threads::Threads)
