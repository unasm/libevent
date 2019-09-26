cmake_minimum_required (VERSION 2.8.11)
project (libevent)


IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

get_filename_component(_RPC_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)


message("_RPC_DIR: ${_RPC_DIR}")
message("CMAKE_FIND_LIBRARY_PREFIXES: ${CMAKE_FIND_LIBRARY_PREFIXES}")
message("RPC_INCLUDES=" ${RPC_INCLUDES})
message("RPC_LIBS=" ${RPC_LIBS})
message("PROJECT_BINARY_DIR=" ${PROJECT_BINARY_DIR})
message("PROJECT_SOURCE_DIR=" ${PROJECT_SOURCE_DIR})
message("CMAKE_CURRENT_SOURCE_DIR=" ${CMAKE_CURRENT_SOURCE_DIR})
#message("punctuation_lib="${punctuation_lib})

set(RPC_LIBS event)

message("RPC_LIBS=" ${RPC_LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -W")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread -Wl,--as-needed")


set(CMAKE_MODULE_PATH "${_RPC_DIR}/cmake/")


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
    /usr/local/include   
)


#add_library(punctuation_lib SHARED)

# target_link_libraries(test_tf
#      punctuation_lib
# )


# set(SRC_LISTS
#     app.cpp
# )

find_package(Libevent REQUIRED)

set(RPC_LIBS event)


#set (RPC_INCLUDES "${_RPC_DIR}/proto" "${_RPC_DIR}/include" ${LIBEVENT_INCLUDE    _DIRS})

set (RPC_INCLUDES ${LIBEVENT_INCLUDE_DIRS})


message("LIBEVENT_INCLUDE_DIRS=" ${LIBEVENT_INCLUDE_DIRS})


#include_directories (libevent)
#add_subdirectory (Dependency)
add_executable (libevent app.cpp)
target_include_directories(libevent PUBLIC ${RPC_INCLUDES})
target_link_libraries(libevent ${RPC_LIBS})

#target_link_libraries(libevent libsugan)

#target_link_libraries (libeve Dependency)