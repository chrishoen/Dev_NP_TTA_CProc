cmake_minimum_required (VERSION 3.8)

project (newcproc CXX)
my_find_src_files (src_files newcproc)
add_executable (newcproc ${src_files})
my_add_compile_options (newcproc)
my_add_pch (newcproc)

my_inc_import_CommonLib(newcproc)
my_inc_import_ShareLib(newcproc)
my_inc_import_RisLib(newcproc)

my_lib_import_RisLib(newcproc)
my_lib_import_ShareLib(newcproc)
my_lib_import_CommonLib(newcproc)

add_custom_command(
   TARGET  newcproc
   POST_BUILD
   COMMAND rsync -azv --chmod=ugo=rwx newcproc rsync://debian@devtgt/prime/bin
   WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_NP_TTA_NewCProc/newcproc)
