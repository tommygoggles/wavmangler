#Be sure to add CMake to the system PATH so that you can use it from the command line. Add it for the current or all users as appropriate. 
#CMake can, of course, be installed as part of Cygwin.
#errors with the cmake-gui may be down to it not finding the compiler... tried visual studio 64 bit 2014? ok?

cmake_minimum_required(VERSION 2.8.9)


#The project command names your project. Optionally you can specify what language the project supports, any of CXX, C, JAVA, or FORTRAN. 
project(wavmangler)



if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)


 
#Bring the headers
#include_directories(include)
 
 
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
 
#However, the file(GLOB...) allows for wildcard additions:
#file(GLOB SOURCES "*.cpp")




#This command tells CMake you want to make an executable and adds it as a target.
#The first argument is the name of the executable and the rest are the source files.
#You may notice that header files aren’t listed. CMake handles dependencies automatically so headers don’t need to be listed. 
add_executable(wavmangler wavmangler.cpp)
#add_executable(tgengine ${SOURCES})
