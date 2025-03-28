# ImGui

set(PROJECT_HEADERS
    External/imgui/imgui.h External/imgui/imconfig.h
    External/imgui/imgui_internal.h External/imgui/imstb_rectpack.h
    External/imgui/imstb_textedit.h External/imgui/imstb_truetype.h)

set(SOURCES
    External/imgui/imgui_demo.cpp External/imgui/imgui_draw.cpp
    External/imgui/imgui_tables.cpp External/imgui/imgui_widgets.cpp
    External/imgui/imgui.cpp)

if(APPLE)
  set(PROJECT_HEADERS
      ${PROJECT_HEADERS} External/imgui/backends/imgui_impl_osx.h
      External/imgui/backends/imgui_impl_metal.h)

  set(SOURCES ${SOURCES} External/imgui/backends/imgui_impl_osx.mm
              External/imgui/backends/imgui_impl_metal.mm)

  find_library(APPKIT AppKit)
  find_library(METAL Metal)
  find_library(METALKIT MetalKit)
  find_library(MODELIO ModelIO)
  find_library(GAMECONTROLLER GameController)

  set(DEPENDENCIES ${APPKIT} ${METAL} ${METALKIT} ${MODELIO} ${GAMECONTROLLER})
elseif(WIN32)
  set(PROJECT_HEADERS
      ${PROJECT_HEADERS} External/imgui/backends/imgui_impl_win32.h
      External/imgui/backends/imgui_impl_opengl3.h)

  set(SOURCES ${SOURCES} External/imgui/backends/imgui_impl_win32.cpp
              External/imgui/backends/imgui_impl_opengl3.cpp)

endif()

add_library(ImGui STATIC ${SOURCES} ${PROJECT_HEADERS})

target_compile_features(ImGui PRIVATE cxx_std_20)

target_include_directories(ImGui PUBLIC External/imgui/)

target_link_libraries(ImGui PUBLIC ${DEPENDENCIES})

if(WIN32)

  set(IMGUI_DEBUGGER_PATH
      ${CMAKE_SOURCE_DIR}/Engine/External/imgui/misc/debuggers)
  set(IMGUI_DEBUG_FILES ${IMGUI_DEBUGGER_PATH}/imgui.natstepfilter
                        ${IMGUI_DEBUGGER_PATH}/imgui.natvis)

  foreach(debug_file IN LISTS IMGUI_DEBUG_FILES)
    get_filename_component(debug_file_name ${debug_file} NAME)
    add_custom_command(
      TARGET ImGui
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${debug_file}
              ${CMAKE_SOURCE_DIR}/build/win
      COMMENT "Copying ${debug_file_name} to build/win directory")
  endforeach()

endif()
