set(SOURCES
    Assert.cpp
    Logger.cpp
    Utils.cpp
    StreamAppConfig.cpp
    SequentialAppConfig.cpp
    OAL.cpp
    Frustum.cpp
    Plane.cpp
    ImageReader.cpp
    ImageWriter.cpp
    BinaryReader.cpp
    FBXParser.cpp
    FBXSceneBuilder.cpp
    FBXMeshBuilder.cpp
    FBXProperties70Builder.cpp
    FBXMaterialTemplateBuilder.cpp
    FBXTextureTemplateBuilder.cpp
    TPS.cpp
    Ray.cpp
    Logger.h
)

add_library(af3dutil STATIC ${SOURCES})

target_link_libraries(af3dutil log4cplus boost stb ddsktx jsoncpp)
