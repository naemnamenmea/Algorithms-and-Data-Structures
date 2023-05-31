file(GLOB SOURCE_FILES tests/*.cpp)
list(APPEND SOURCE_FILES
    data_structuresUnitTests.cpp)

add_executable(data_structuresUnitTests ${SOURCE_FILES})

target_link_libraries(data_structuresUnitTests PUBLIC data_structures)
target_link_libraries(data_structuresUnitTests PUBLIC my_testing_tools)