cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(imu)

include_directories(../ioData/src ../odm/src)

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

add_executable(imu-test ./src/imu-test.cpp ./src/GraphDrawerIMU.cpp ../ioData/src/inpData_imu.cpp ../ioData/src/ioData_base.cpp)
target_link_libraries(imu-test m)

add_executable(imu-est ./src/imu-est.cpp ./src/GraphDrawerIMU.cpp ../ioData/src/inpData_imu.cpp ../ioData/src/ioData_base.cpp ../odm/src/GraphDrawerODM.cpp)
target_link_libraries(imu-est m)

