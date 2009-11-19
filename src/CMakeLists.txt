
set(ample_SOURCES AmpleBitmap.cpp Ample.cpp AmpleGeometry.cpp AmpleMaterial.cpp
                  AmpleNode.cpp AmpleObject.cpp AmpleSession.cpp AmpleTag.cpp
                  AmpleText.cpp)

add_library(ample STATIC ${ample_SOURCES})

install(TARGETS ample ARCHIVE DESTINATION lib)

