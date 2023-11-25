find_library(glew_dbg_lib NAMES glew32 REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/win/Debug/lib" NO_DEFAULT_PATH)
find_path(glew_dbg_include "GL/glew.h" REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/win/Debug/include" NO_DEFAULT_PATH)

find_library(glew_opt_lib NAMES glew32 REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/win/Release/lib" NO_DEFAULT_PATH)
find_path(glew_opt_include "GL/glew.h" REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/win/Release/include" NO_DEFAULT_PATH)
