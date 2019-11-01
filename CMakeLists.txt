cmake_minimum_required(VERSION 3.9)

project(InferenceEngineAddon)

set(CMAKE_CXX_STANDARD 11)

find_package(InferenceEngine 2.1 REQUIRED)

include_directories("include" "${InferenceEngine_INCLUDE_DIRS}")

file(GLOB SOURCE_FILES
        src/*.cpp
        src/*.c)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/node_modules/node-addon-api
        ${CMAKE_SOURCE_DIR}/node_modules/node-addon-api/src
        )

target_link_libraries(${PROJECT_NAME} PRIVATE ${InferenceEngine_LIBRARIES})
