#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_init_global_import_variables)

   if(MSVC)
      set (MyRisLibIncludePath "C:\\MyTools\\MyLib\\include\\RisLib" PARENT_SCOPE)
      set (MyRisLibImportPath  "C:\\MyTools\\MyLib\\lib\\RisLib.lib" PARENT_SCOPE)
      set (MyDspLibIncludePath "C:\\MyTools\\MyLib\\include\\DspLib" PARENT_SCOPE)
      set (MyDspLibImportPath  "C:\\MyTools\\MyLib\\lib\\DspLib.lib" PARENT_SCOPE)
   elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
      set (MyRisLibIncludePath "/usr/local/include/RisLib" PARENT_SCOPE)
      set (MyRisLibImportPath  "/usr/local/lib/libRisLib.so" PARENT_SCOPE)
      set (MyDspLibIncludePath "/usr/local/include/DspLib" PARENT_SCOPE)
      set (MyDspLibImportPath  "/usr/local/lib/libDspLib.a" PARENT_SCOPE)
      set (MyPhpCppIncludePath "/usr/include" PARENT_SCOPE)
      set (MyPhpCppImportPath  "/usr/lib/libphpcpp.so" PARENT_SCOPE)
   else()
      set (MyRisLibIncludePath "C:/MyTools/MyLib/include/RisLib" PARENT_SCOPE)
      set (MyRisLibImportPath  "C:/MyTools/MyLib/lib/libRisLib.so" PARENT_SCOPE)
      set (MyDspLibIncludePath "C:/MyTools/MyLib/include/DspLib" PARENT_SCOPE)
      set (MyDspLibImportPath  "C:/MyTools/MyLib/lib/libDspLib.a" PARENT_SCOPE)
      set (MyPhpCppIncludePath "C:/Beagle/phpcpp/include" PARENT_SCOPE)
      set (MyPhpCppImportPath  "C:/Beagle/phpcpp/lib/libphpcpp.so" PARENT_SCOPE)
   endif()

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_RisLib _target)

   if (MSVC)
      add_library(RisLib STATIC IMPORTED)
      set_target_properties(RisLib PROPERTIES IMPORTED_LOCATION ${MyRisLibImportPath})
      target_link_libraries(RisLib INTERFACE ws2_32)
      target_link_libraries(RisLib INTERFACE winmm)
      target_link_libraries(${_target} RisLib)
   else()
      add_library(RisLib SHARED IMPORTED)
      set_target_properties(RisLib PROPERTIES IMPORTED_LOCATION ${MyRisLibImportPath})
      target_link_libraries(${_target} RisLib)
   endif()

endfunction()

function(my_inc_import_RisLib _target)

   target_include_directories(${_target} PUBLIC ${MyRisLibIncludePath})

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_DspLib _target)

   add_library(DspLib STATIC IMPORTED)
   set_target_properties(DspLib PROPERTIES IMPORTED_LOCATION ${MyDspLibImportPath})
   target_link_libraries(${_target} DspLib)

endfunction()

function(my_inc_import_DspLib _target)

   target_include_directories(${_target} PUBLIC ${MyDspLibIncludePath})

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_phpcpp _target)

   add_library(phpcpp SHARED IMPORTED)
   set_target_properties(phpcpp PROPERTIES IMPORTED_LOCATION ${MyPhpCppImportPath})
   target_link_libraries(${_target} phpcpp)
   
endfunction()

function(my_inc_import_phpcpp _target)

   target_include_directories(${_target} PUBLIC ${MyPhpCppIncludePath})

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
