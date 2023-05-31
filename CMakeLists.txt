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