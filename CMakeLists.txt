cmake_minimum_required (VERSION 2.6)

project(AoC2018)

LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/lib )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_library(ops STATIC
    lib/operations.cpp)

set(COMMON_LIBS ops)

set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(USERS
    sellersgrant)

set(DAYS
    Day01
    Day02
    Day03
    Day04
    Day05
    Day06
    Day07
    Day08
    Day09
    Day10
    Day11
    Day12
    Day13
    Day14
    Day15
    Day16
    Day17
    Day18
    Day19
    Day20
    Day21
    Day22
    Day23
    Day24
    Day25
)
foreach(USER ${USERS})

  foreach(DAY ${DAYS})
    add_executable(${USER}${DAY} src/${DAY}/${USER}.cpp)
    set_property(TARGET ${USER}${DAY} PROPERTY DEBUG_POSTFIX _d)
    target_link_libraries(${USER}${DAY} ${COMMON_LIBS})
    if(MSVC)
      configure_file(${PROJECT_SOURCE_DIR}/build/templates/vs2013.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${DAY}${USER}.vcxproj.user @ONLY)
    endif(MSVC)
  endforeach(DAY)

endforeach(USER)
IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_LINUX")
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories( include )

ADD_CUSTOM_TARGET(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${PROJECT_SOURCE_DIR})
ADD_CUSTOM_TARGET(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${PROJECT_SOURCE_DIR})
