cmake_minimum_required (VERSION 3.8)

set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/MyOverrides.cmake")
include(MyPrecompiledHeader.cmake)
include(MyCodeMacros.cmake)
include(MyGlobalImports.cmake)
include(MyLocalImports.cmake)

project ("Dev_NP_TTA_NewCProc")
my_init_global_import_variables()

add_subdirectory ("CommonLib")
add_subdirectory ("newcproc")
return()

add_subdirectory ("CommonLib")
add_subdirectory ("commtest")
return()

add_subdirectory ("Test")
return()


