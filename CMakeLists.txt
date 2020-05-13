cmake_minimum_required(VERSION 3.9)

project(inference_engine_js)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-frtti -fexceptions")

find_package(InferenceEngine 2.1 REQUIRED)

set(NODE_NODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/node_modules")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${InferenceEngine_INCLUDE_DIRS}"
                    "${NODE_NODULES_PATH}/node-addon-api"
                    "$ENV{NODE_PATH}/include/node"
                    )

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}")

target_link_libraries(${PROJECT_NAME} PRIVATE ${InferenceEngine_LIBRARIES})

add_definitions(-DNAPI_VERSION=4)