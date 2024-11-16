#################################################
#
#  (C) 2011 Golubev Alexander
#  fatzer2 (AT) gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

add_definitions(
  -DQT_PLUGIN
)

include_directories(
  ${TQT_INCLUDE_DIRS}
  ${TDE_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)

##### other data ################################

tde_create_translated_desktop(
  SOURCE oxygen.themerc
  DESTINATION ${DATA_INSTALL_DIR}/tdestyle/themes
  PO_DIR style-themes
)


##### dotnet #####################################

set( target oxygenstyle )

set( ${target}_SRCS
  oxygenstyle.cpp misc.cpp
)

tde_add_kpart( ${target} AUTOMOC
  SOURCES ${${target}_SRCS}
  LINK tdeui-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/styles
)
