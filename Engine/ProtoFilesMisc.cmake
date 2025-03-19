function(GetProtoFiles)
    set(GLOB_EXPRESSION "*.proto")

    cmake_parse_arguments(ARG "" "" "PATHS" ${ARGN})

    if (NOT DEFINED ARG_PATHS)
        message(WARNING "No Proto files folder paths provided!")
        return()
    endif()

    message(STATUS "Proto files folders: ${ARG_PATHS}")

    set(PROTO_FILES "")

    foreach(proto_path ${ARG_PATHS})
        file(GLOB proto_files "${proto_path}/${GLOB_EXPRESSION}")

        foreach(proto ${proto_files})
            message(STATUS "Found proto file: ${proto}")
        endforeach()

        list(APPEND PROTO_FILES ${proto_files})
    endforeach()

    set(PROTO_FILES ${PROTO_FILES} PARENT_SCOPE)
endfunction()

function(GetProtoHeaderFiles)
    set(GLOB_EXPRESSION "*.pb.h")
    set(OUT_HEADER_FILES "")

    cmake_parse_arguments(ARG "" "PROTO_OUT" "" ${ARGN})

    if (NOT DEFINED ARG_PROTO_OUT)
        message(WARNING "No proto output folder provided!")
        return()
    endif()

    file(GLOB HEADER_FILES ${ARG_PROTO_OUT}/${GLOB_EXPRESSION})
    foreach(header ${HEADER_FILES})
        message(STATUS "Proto header file: ${header}")
        file(TO_CMAKE_PATH "${header}" header)
        list(APPEND OUT_HEADER_FILES ${header})
    endforeach()
    set(OUT_HEADER_FILES ${OUT_HEADER_FILES} CACHE INTERNAL "Proto header files")
    return()
endfunction()

function(GetProtoSourceFiles)
    set(GLOB_EXPRESSION "*.pb.cc")
    set(OUT_SOURCE_FILES "")

    cmake_parse_arguments(ARG "" "PROTO_OUT" "" ${ARGN})
    if (NOT DEFINED ARG_PROTO_OUT)
        message(WARNING "No proto output folder provided!")
        return()
    endif()

    file(GLOB SOURCE_FILES ${ARG_PROTO_OUT}/${GLOB_EXPRESSION})
    foreach(source ${SOURCE_FILES})
        message(STATUS "Proto source file: ${source}")
        file(TO_CMAKE_PATH "${source}" source)
        list(APPEND OUT_SOURCE_FILES ${source})
    endforeach()
    set(OUT_SOURCE_FILES ${OUT_SOURCE_FILES} CACHE INTERNAL "Proto header files")
    return()
endfunction()

