# GLAD

set ( GLAD_INCLUDES
	include/glad/glad.h
	include/KHR/khrplatform.h
	src/glad.c
)

add_library( Glad
    STATIC
    ${GLAD_INCLUDES}
)

target_include_directories(Glad 
	PUBLIC 
	include
)

target_compile_features(Glad 
	PUBLIC
	c_std_99
)

set_target_properties(Glad 
	PROPERTIES 
	LINKER_LANGUAGE C
)