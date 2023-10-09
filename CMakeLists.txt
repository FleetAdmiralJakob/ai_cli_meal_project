cmake_minimum_required(VERSION 3.26)
project(ai_cli_meal_project C)

set(CMAKE_C_STANDARD 17)

enable_language(CXX)

find_package(CURL REQUIRED)
find_package(jansson REQUIRED)

add_executable(ai_cli_meal_project main.c
        env_reader.h
        env_reader.c)

target_link_libraries(ai_cli_meal_project CURL::libcurl jansson::jansson)
