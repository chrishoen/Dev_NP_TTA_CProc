cmake_minimum_required (VERSION 3.8)

project (cproc CXX)
my_find_src_files (src_files cproc)
add_executable (cproc ${src_files})
my_add_compile_options (cproc)
my_add_pch (cproc)

my_inc_import_CommonLib(cproc)
my_inc_import_ShareLib(cproc)
my_inc_import_RisLib(cproc)
my_inc_import_EventLib(cproc)

my_lib_import_RisLib(cproc)
my_lib_import_ShareLib(cproc)
my_lib_import_CommonLib(cproc)
my_lib_import_EventLib(cproc)

add_custom_command(
   TARGET  cproc
   POST_BUILD
   COMMAND rsync -azv --chmod=ugo=rwx cproc rsync://debian@devtgt/prime/bin
   WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_NP_TTA_CProc/cproc)
