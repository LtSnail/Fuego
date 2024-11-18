project ( Glad )

set( SOURCES
    src/gl.c
    src/wgl.c
)

set( HEADERS
    include/glad/gl.h
	include/glad/wgl.h
)

add_library ( Glad 
	STATIC 
	${HEADERS}
 	${SOURCES}
 )

target_include_directories ( Glad PUBLIC include/)