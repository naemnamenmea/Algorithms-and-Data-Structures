cmake_minimum_required(VERSION 3.19)

include(FetchContent)

FetchContent_Declare(
    cmake-tutorial
    GIT_REPOSITORY https://github.com/naemnamenmea/cmake-tutorial.git
    GIT_TAG        main
)
FetchContent_Declare(
    math
    GIT_REPOSITORY https://github.com/naemnamenmea/math.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(math cmake-tutorial)

option(MW_PROJECT_NAME_FROM_DIRECTORY "Use the directory name  of the source directory as project name" OFF)
if(MW_PROJECT_NAME_FROM_DIRECTORY)
	mw_set_project_name_from_source_dir(project_name)
else()
	set(project_name "Solution")
endif()
project(${project_name} LANGUAGES C CXX)

if (MSVC)
    # warning level 4
    add_compile_options(/W4 /WX)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

set(CMAKE_CXX_STANDARD 17)
# set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# set(CMAKE_SUPPRESS_REGENERATION OFF)

# option(MY_USE_UNITY_BUILD "use unity build" OFF)
# option(MY_RUN_TESTS "enable testing" ON)

add_compile_definitions(
    LOCAL_LAUNCH
    INPUT_FILENAME="input.txt"
    OUTPUT_FILENAME="output.txt"
    EXPECTED_FILENAME="expected.txt"
)
if(MY_RUN_TESTS)
    add_compile_definitions(LOCAL_RUN)
endif()

file(GLOB SOURCE_FILES
    *.cpp
    src/*.cpp
    *.hpp
    *.h
    public/*.hpp
    public/*.h
    private/*.hpp
    private/*.h)

add_library(data_structures STATIC ${SOURCE_FILES})

target_link_libraries(data_structures PUBLIC math)

target_include_directories(data_structures PUBLIC public)
target_include_directories(data_structures PRIVATE private)

add_subdirectory(data_structuresUnitTests)