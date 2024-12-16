add_library(FuegoShaderTranslatorInterface INTERFACE)
target_include_directories(FuegoShaderTranslatorInterface INTERFACE ${FUEGO_DXC_PATH}/include)
target_link_libraries(FuegoShaderTranslatorInterface INTERFACE LLVMDxcSupport dxcompiler)
target_compile_features(FuegoShaderTranslatorInterface INTERFACE cxx_std_20)
target_compile_definitions(FuegoShaderTranslatorInterface INTERFACE FUEGO_ENABLE_ASSERTS _ITERATOR_DEBUG_LEVEL=0)
if (MSVC)
    target_compile_options(FuegoShaderTranslatorInterface INTERFACE /W4 /WX /permissive- /utf-8)
else()
    target_compile_options(FuegoShaderTranslatorInterface INTERFACE -Wall -Wextra -Werror)
endif()


set(FUEGO_DXC_PATH External/DirectXShaderCompiler)
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "")
set(CLANG_INCLUDE_TESTS OFF CACHE BOOL "")
set(HLSL_INCLUDE_TESTS OFF CACHE BOOL "")
set(SPIRV_BUILD_TESTS OFF CACHE BOOL "")
set(HLSL_COPY_GENERATED_SOURCES OFF CACHE BOOL "")
set(HLSL_DISABLE_SOURCE_GENERATION ON CACHE BOOL "")
include(${FUEGO_DXC_PATH}/cmake/caches/PredefinedParams.cmake)
add_subdirectory(${FUEGO_DXC_PATH} EXCLUDE_FROM_ALL)


set(PROJECT_HEADERS include/Fuego/ShaderTranslator.hpp)
set(SOURCES ShaderTranslator.cpp)

add_library(FuegoShaderTranslator ${SOURCES} ${PROJECT_HEADERS})
target_include_directories(FuegoShaderTranslator PUBLIC include PRIVATE include ${FUEGO_DXC_PATH}/include)
target_link_libraries(FuegoShaderTranslator PUBLIC FuegoShaderTranslatorInterface PRIVATE LLVMDxcSupport dxcompiler)

add_executable(FuegoShaderTranslatorCl Main.cpp)
target_link_libraries(FuegoShaderTranslatorCl FuegoShaderTranslator)
