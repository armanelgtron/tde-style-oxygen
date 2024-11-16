
include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
  ${TDE_LIBRARY_DIRS}
)


##### twin_SUSE2_config (kpart)

tde_add_kpart( twin_oxygen_config AUTOMOC

  SOURCES
        exampleconfig.cpp
        configdialog.ui
  LINK
     tdecore-shared
     tdeui-shared
     tdeio-shared

  DESTINATION ${PLUGIN_INSTALL_DIR}
) 

