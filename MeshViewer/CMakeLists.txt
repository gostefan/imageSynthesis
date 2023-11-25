add_executable(MeshViewer main.cpp MeshViewer.cpp)

target_link_system_libraries(
  MeshViewer
  PRIVATE
          freeglut_static)

target_link_libraries(MeshViewer PRIVATE debug "${glew_dbg_lib}" optimized "${glew_opt_lib}")

target_include_directories(MeshViewer PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include" "${CMAKE_SOURCE_DIR}" "${glew_dbg_include}")
