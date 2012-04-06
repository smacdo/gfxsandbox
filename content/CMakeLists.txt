set(src_root ${CMAKE_SOURCE_DIR}/content)
set(dest_root ${CMAKE_BINARY_DIR}/content)

file(INSTALL
    ${src_root}/images/hello1.tga
    ${src_root}/images/hello2.tga
    DESTINATION
    ${dest_root}/images)

file(INSTALL
    ${src_root}/shaders/hello.ps.glsl
    ${src_root}/shaders/hello.vs.glsl
    DESTINATION
    ${dest_root}/shaders)
