cmake_minimum_required(VERSION 3.9)

project(InferenceEngineJS)

set(CMAKE_CXX_STANDARD 11)

find_package(InferenceEngine 2.1 REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${InferenceEngine_INCLUDE_DIRS}"
                    "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-addon-api"
                    "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-addon-api/src")

file(GLOB SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
        )

add_compile_definitions(NAPI_DISABLE_CPP_EXCEPTIONS)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

target_link_libraries(${PROJECT_NAME} PRIVATE ${InferenceEngine_LIBRARIES})
