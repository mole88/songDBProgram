cmake_minimum_required(VERSION 3.16)
project(dbProgram C)

set(CMAKE_C_STANDARD 99)
add_executable(dbProgram main.c)

set(FILE_TO_COPY "db.csv")

add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${FILE_TO_COPY}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${FILE_TO_COPY} ${CMAKE_BINARY_DIR}/${FILE_TO_COPY}
        DEPENDS ${CMAKE_SOURCE_DIR}/${FILE_TO_COPY}
)

add_custom_target(CopyFile ALL DEPENDS ${CMAKE_BINARY_DIR}/${FILE_TO_COPY})
add_dependencies(${PROJECT_NAME} CopyFile)