# Glad

set(PROJECT_HEADERS include/glad/gl.h include/glad/wgl.h
                    include/KHR/khrplatform.h)

set(PROJECT_SOURCES src/gl.c src/wgl.c)

add_library(Glad STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

set_target_properties(Glad PROPERTIES LANGUAGES C)

target_include_directories(Glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Glad PRIVATE opengl32)
