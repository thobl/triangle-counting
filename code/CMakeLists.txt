cmake_minimum_required(VERSION 3.10)

project(TriangleCounting)

# setting the build type to debug when in the debug directory and to
# release otherwise
get_filename_component(build_dir ${CMAKE_CURRENT_BINARY_DIR} NAME)
if (${build_dir} MATCHES "debug")
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

# c++ version 20
set(CMAKE_CXX_STANDARD 20)

# export compile_commands.json, which can be used by editors for auto
# completion (e.g., using lsp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# helping the compiler to find header files
include_directories(include lib)

# header, source and executable files
set(HEADERS
  lib/CLI11.hpp
  include/app.hpp
  include/timer.hpp
  include/random.hpp
  include/graph.hpp
  include/triangles.hpp
  )

set(SOURCE_FILES
  src/timer.cpp
  src/random.cpp
  src/graph.cpp
  src/triangles.cpp
  )

set(EXECUTABLE_FILES
  cli/girg.cpp
  cli/count_triangles.cpp
  )

# create a target for every executable linking all source files
add_library(code STATIC ${SOURCE_FILES} ${HEADERS})
foreach (file ${EXECUTABLE_FILES})
    get_filename_component(TARGET ${file} NAME_WE)
    add_executable(${TARGET} ${file})
    target_link_libraries(${TARGET} code)

    # show all warnings
    if(MSVC)
      target_compile_options(${TARGET} PRIVATE /W4 /WX)
    else()
      target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach ()

######################################################################
# use girgs library
add_subdirectory(../girgs girgs)

target_include_directories(girg PRIVATE ../girgs/source/girgs/include)
target_link_libraries(girg girgs)
