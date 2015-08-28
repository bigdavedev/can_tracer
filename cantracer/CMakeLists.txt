get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME )
project ( ${ProjectId} )

##---------------------------------------------------------------------------##
# library dependencies
# ====================
# We use the following libraries:
# * sqlite3
##---------------------------------------------------------------------------##
set ( LIBRARIES cantools )

include_directories ( include )

include_directories ( ${CMAKE_SOURCE_DIR}/cantools/include )

set ( SOURCES source/main.cpp )

add_definitions ( "-g")

add_executable ( ${PROJECT_NAME} ${SOURCES} )
target_link_libraries ( ${PROJECT_NAME} ${LIBRARIES} )
