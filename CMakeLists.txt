cmake_minimum_required(VERSION 3.1.3)

project(prac C)

# Set standart
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Compile commands for lang server
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set GCC compile options
add_compile_options(
	-g -O2 -pipe 
	-D_FORTIFY_SOURCE=2 
	-Werror=implicit-function-declaration -Wall
	-funsigned-char -std=c11 -fms-extensions
)


set(INCLUDE_DIR include)

set(DIRS 
	${INCLUDE_DIR}
)

# Set libs
add_library(base STATIC
	${INCLUDE_DIR}/Utils.c
	${INCLUDE_DIR}/Sort.c
	${INCLUDE_DIR}/Any.c
	${INCLUDE_DIR}/Args.c
)

target_include_directories(base PUBLIC ${DIRS})

# Exec
set(EXECUTABLES
	task1
	task1_3
	task2
	task3_v2
	task4
)

foreach(EXEC IN LISTS EXECUTABLES)
	add_executable(${EXEC} ${EXEC}.c)
	target_link_libraries(${EXEC} base)
endforeach()
