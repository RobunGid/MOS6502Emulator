cmake_minimum_required(VERSION 3.7)

project( M6502Lib )

if(MSVC)
	add_compile_options(/MP)				#Use multiple processors when building
	add_compile_options(/W4 /wd4201 /WX)	#Warning level 4, all warnings are errors
else()
	add_compile_options(-W -Wall -Werror) #All Warnings, all warnings are errors
endif()

set  (M6502_SOURCES
    "src/public/m6502.h"
    "src/private/m6502.cpp"
	"src/private/m6502.cpp")
		
source_group("src" FILES ${M6502_SOURCES})
		
add_library( MOS6502Lib ${M6502_SOURCES} )

target_include_directories ( MOS6502Lib PUBLIC "${PROJECT_SOURCE_DIR}/src/public")
target_include_directories ( MOS6502Lib PRIVATE "${PROJECT_SOURCE_DIR}/src/private")

#set_target_properties(MOS6502Lib PROPERTIES FOLDER "MOS6502Lib")