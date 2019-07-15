cmake_minimum_required(VERSION 3.14)
project(Compilador)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compilador main.cpp Semantico.cpp Semantico.h Token.cpp Token.h)


SET (BOOST_ROOT "c:/mingw/include/boost")
SET (BOOST_INCLUDEDIR "c:/mingw/include/boost/include")
SET (BOOST_LIBRARYDIR "c:/mingw/include/boost/lib")

SET (BOOST_MIN_VERSION "1.70.0")
set (Boost_NO_BOOST_CMAKE ON)
FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Fatal error: Boost (version >= 1.55) required.")
else()
    message(STATUS "Setting up BOOST")
    message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
    message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif (NOT Boost_FOUND)
