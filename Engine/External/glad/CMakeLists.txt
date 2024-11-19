project ( Glad )

set( SOURCES
    src/glad.c
)

set( HEADERS
    include/glad/glad.h
    include/KHR/khrplatform.h
)

add_library ( Glad 
	STATIC 
	${HEADERS}
 	${SOURCES}
 )

target_include_directories ( Glad 
	PUBLIC
	include/
)

target_compile_features( Glad PUBLIC cxx_std_20 )