#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_init_global_import_variables)

   set (MyRisLibIncludePath "C:/MyTools/MyLib/include/RisLib" PARENT_SCOPE)
   set (MyRisLibImportPath  "C:/MyTools/MyLib/lib/libRisLib.so" PARENT_SCOPE)
   set (MyDspLibIncludePath "C:/MyTools/MyLib/include/DspLib" PARENT_SCOPE)
   set (MyDspLibImportPath  "C:/MyTools/MyLib/lib/libDspLib.a" PARENT_SCOPE)
   set (MyShareLibIncludePath "C:/MyTools/MyLib/include/ShareLib" PARENT_SCOPE)
   set (MyShareLibImportPath  "C:/MyTools/MyLib/lib/libShareLib.so" PARENT_SCOPE)
   set (MyEventLibIncludePath "C:/MyTools/MyLib/include/EventLib" PARENT_SCOPE)
   set (MyEventLibImportPath  "C:/MyTools/MyLib/lib/libEventLib.so" PARENT_SCOPE)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_RisLib _target)

   add_library(RisLib SHARED IMPORTED)
   set_target_properties(RisLib PROPERTIES IMPORTED_LOCATION ${MyRisLibImportPath})
   target_link_libraries(${_target} RisLib)

endfunction()

function(my_inc_import_RisLib _target)

   target_include_directories(${_target} PUBLIC ${MyRisLibIncludePath})

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_DspLib _target)

   add_library(DspLib SHARED IMPORTED)
   set_target_properties(DspLib PROPERTIES IMPORTED_LOCATION ${MyDspLibImportPath})
   target_link_libraries(${_target} DspLib)

endfunction()

function(my_inc_import_DspLib _target)

   target_include_directories(${_target} PUBLIC ${MyDspLibIncludePath})

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_ShareLib _target)

   add_library(ShareLib SHARED IMPORTED)
   set_target_properties(ShareLib PROPERTIES IMPORTED_LOCATION ${MyShareLibImportPath})

   target_link_libraries(${_target} ShareLib)

endfunction()

function(my_inc_import_ShareLib _target)

   target_include_directories(${_target} PUBLIC ${MyShareLibIncludePath})

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_EventLib _target)

   add_library(EventLib SHARED IMPORTED)
   set_target_properties(EventLib PROPERTIES IMPORTED_LOCATION ${MyEventLibImportPath})

   target_link_libraries(${_target} EventLib)

endfunction()

function(my_inc_import_EventLib _target)

   target_include_directories(${_target} PUBLIC ${MyEventLibIncludePath})

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************
