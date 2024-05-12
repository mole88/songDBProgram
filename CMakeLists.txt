cmake_minimum_required(VERSION 3.16)
project(dbProgram C)

set(CMAKE_C_STANDARD 99)
add_executable(dbProgram main.c)

set(DB_FILE "db.csv")
set(INFO_FILE "README")

add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${DB_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${DB_FILE} ${CMAKE_BINARY_DIR}/${DB_FILE}
        DEPENDS ${CMAKE_SOURCE_DIR}/${DB_FILE}
)
add_custom_target(CopyDbFile ALL DEPENDS ${CMAKE_BINARY_DIR}/${DB_FILE})
add_dependencies(${PROJECT_NAME} CopyDbFile)

add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${INFO_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${INFO_FILE} ${CMAKE_BINARY_DIR}/${INFO_FILE}
        DEPENDS ${CMAKE_SOURCE_DIR}/${INFO_FILE}
)
add_custom_target(CopyInfoFile ALL DEPENDS ${CMAKE_BINARY_DIR}/${INFO_FILE})
add_dependencies(${PROJECT_NAME} CopyInfoFile)