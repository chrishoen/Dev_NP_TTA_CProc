cmake_minimum_required (VERSION 3.8)

project (commtest CXX)
my_find_src_files (src_files commtest)
add_executable (commtest ${src_files})
my_add_compile_options (commtest)
my_add_pch (commtest)

my_inc_import_RisLib(commtest)

my_lib_import_RisLib(commtest)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS commtest DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  commtest
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx commtest rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_CommTest/commtest)
endif()
