CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(py-libssh)

SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

FIND_PACKAGE(PythonInterp)
FIND_PACKAGE(PythonLibs ${PYTHON_VERSION_STRING} EXACT)
#MESSAGE(${PYTHON_VERSION_STRING})

FIND_LIBRARY(LIBSSH ssh /usr/lib /usr/local/lib)
IF (PYTHON_VERSION_MAJOR EQUAL 3)
    FIND_PACKAGE(Boost COMPONENTS python3 filesystem)
ELSE()
    FIND_PACKAGE(Boost COMPONENTS python filesystem)
ENDIF()

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

SET(CMAKE_CXX_FLAGS "-O3 -std=c++11")

SET(PYSSH_HEADERS
    bytes_converter.hpp
    bytes.hpp
    channel.hpp
    #exception.hpp
    result.hpp
    sftp.hpp
    sftp_file.hpp
    ssh.hpp
)

SET(PYSSH_SOURCE
    bytes_converter.cpp
    bytes.cpp
    channel.cpp
    result.cpp
    #exception.cpp
    pyssh.cpp
    sftp.cpp
    sftp_file.cpp
    ssh.cpp
)

ADD_LIBRARY(pyssh SHARED ${PYSSH_SOURCE})
TARGET_LINK_LIBRARIES(pyssh ${LIBSSH} ${Boost_LIBRARIES})
SET_TARGET_PROPERTIES(pyssh PROPERTIES
    OUTPUT_NAME "_pyssh"
    PREFIX ""
)
