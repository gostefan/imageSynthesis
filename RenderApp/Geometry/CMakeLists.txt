target_sources(RenderApp PRIVATE
	BBox.cpp
	BicubicPatch.cpp
	Mesh.cpp
	MicroGrid.cpp
	MicroPolygon.cpp
	Motion.cpp
	Rain.cpp
	Shape.cpp
	Sphere.cpp
	TranslatedMotion.cpp
)
add_subdirectory(Displacement)
add_subdirectory(Texture)
