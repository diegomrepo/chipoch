cmake_minimum_required(VERSION 3.10)
project(chipoch VERSION 0.1)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_compile_options(-W -Wall -Wextra -pedantic -pedantic-errors)

add_library(chip-8 STATIC chip-8.c chip-8.h)
add_executable(chipoch main.c)

#target_compile_options(chipoch PUBLIC -W -Wall -Wextra -pedantic -pedantic-errors)
#target_compile_options(chip-8 PUBLIC -W -Wall -Wextra -pedantic -pedantic-errors)

target_link_libraries(chipoch chip-8 ${SDL2_LIBRARIES})

# Run after building
add_custom_target(run ALL COMMAND chipoch)

# Optional installing
install(TARGETS chipoch DESTINATION $ENV{HOME}/bin)

