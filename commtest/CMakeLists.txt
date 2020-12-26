cmake_minimum_required (VERSION 3.8)

project (commtest CXX)
my_find_src_files (src_files commtest)
add_executable (commtest ${src_files})
my_add_compile_options (commtest)
my_add_pch (commtest)

my_inc_import_CommonLib(commtest)
my_inc_import_ShareLib(commtest)
my_inc_import_RisLib(commtest)

my_lib_import_RisLib(commtest)
my_lib_import_ShareLib(commtest)
my_lib_import_CommonLib(commtest)

add_custom_command(
   TARGET  commtest
   POST_BUILD
   COMMAND rsync -azv --chmod=ugo=rwx commtest rsync://debian@192.168.1.72/prime/bin
   WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_NP_TTA_NewCProc/commtest)
