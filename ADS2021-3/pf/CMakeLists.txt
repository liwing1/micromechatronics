cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(pf)

include_directories(../ioData/src)

# set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# set the build type
set(CMAKE_BUILD_TYPE Release)
# confirmation messages
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

#add_executable(pf-test ./src/pf-test.cpp ./src/GraphDrawerPF.cpp ../ioData/src/inpData_odm.cpp ../ioData/src/ioMap.cpp ../ioData/src/ioData_base.cpp)
add_executable(pf-test ./src/pf-test.cpp ./src/GraphDrawerPF.cpp ../ioData/src/inpData_odm.cpp ../ioData/src/inpData_urg.cpp  ../ioData/src/ioMap.cpp ../ioData/src/ioData_base.cpp)
target_link_libraries(pf-test m)

