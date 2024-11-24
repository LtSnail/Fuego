set(FUEGO_DXC_PATH External/DirectXShaderCompiler)

set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "")
set(CLANG_INCLUDE_TESTS OFF CACHE BOOL "")
set(HLSL_INCLUDE_TESTS OFF CACHE BOOL "")
set(SPIRV_BUILD_TESTS OFF CACHE BOOL "")
include(${FUEGO_DXC_PATH}/cmake/caches/PredefinedParams.cmake)
add_subdirectory(${FUEGO_DXC_PATH} EXCLUDE_FROM_ALL)

set(PROJECT_HEADERS)
set(SOURCES Main.cpp)
add_executable(FuegoShaderTranslator ${SOURCES} ${PROJECT_HEADERS})

target_include_directories(FuegoShaderTranslator PUBLIC ${FUEGO_DXC_PATH}/include)
target_link_libraries(FuegoShaderTranslator dxcompiler)

target_compile_features(FuegoShaderTranslator PUBLIC cxx_std_17)
target_compile_definitions(FuegoShaderTranslator PUBLIC FUEGO_ENABLE_ASSERTS)

if (MSVC)
    target_compile_options(FuegoShaderTranslator PRIVATE /W4 /WX /permissive- /utf-8)
else()
    target_compile_options(FuegoShaderTranslator PRIVATE -Wall -Wextra -Werror)
endif()
