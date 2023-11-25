add_executable(MeshViewer main.cpp MeshViewer.cpp)

target_link_libraries(RenderApp PRIVATE debug "${glew_dbg_lib}" optimized "${glew_opt_lib}")

target_include_directories(RenderApp PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include" "${glew_opt_include}")
