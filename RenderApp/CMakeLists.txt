add_executable(RenderApp main.cpp RendererApp.cpp)

add_subdirectory(Geometry)
add_subdirectory(Renderer)
add_subdirectory(Scene)
add_subdirectory(Shader)

#target_link_libraries(
#  intro
#  PRIVATE myproject::myproject_options
#          myproject::myproject_warnings)

#target_link_system_libraries(
#  intro
#  PRIVATE
#          CLI11::CLI11
#          fmt::fmt
#          spdlog::spdlog
#          lefticus::tools
#          ftxui::screen
#          ftxui::dom
#          ftxui::component)

target_link_system_libraries(
  RenderApp
  PRIVATE
          freeglut_static)

target_link_libraries(RenderApp PRIVATE debug "${glew_dbg_lib}" optimized "${glew_opt_lib}")

target_include_directories(RenderApp PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include" "${CMAKE_SOURCE_DIR}" "${glew_opt_include}")
