# Glad

set(PROJECT_HEADERS External/glad/include/glad/glad.h
                    External/glad/include/KHR/khrplatform.h)

set(SOURCES External/glad/src/glad.c)

add_library(Glad STATIC ${SOURCES} ${PROJECT_HEADERS})

target_include_directories(Glad PRIVATE External/glad/include)

set_target_properties(Glad PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES)

target_link_libraries(Glad PRIVATE opengl32)
