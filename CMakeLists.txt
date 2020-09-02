# CMakeList.txt : CMake project for MCGeForce, include source and define # project specific logic here. # cmake_minimum_required (VERSION 3.8) project ("MCGeForce") # Add source to this project's executable. add_executable (MCGeForce "MCGeForce.cpp" "MCGeForce.h") # TODO: Add tests and install targets if needed.

cmake_minimum_required (VERSION 3.8)

project ("MCGeForce")

# Add source to this project's executable.
add_library (MCGeForce SHARED "MCGeForce.cpp" "MCGeForce.h" "JNIHelper.cpp")
# add_executable (MCGeForce "MCGeForce.cpp" "MCGeForce.h" )

# TODO: Add tests and install targets if needed.

target_include_directories(MCGeForce PUBLIC "D:/Development/C++/MCGeForce/include" "C:/Program Files/Java/jdk1.8.0_211/include" "C:/Program Files/Java/jdk1.8.0_211/include/win32")
target_link_directories(MCGeForce PUBLIC D:/Development/C++/MCGeForce/lib)
target_link_libraries(MCGeForce gfesdk)