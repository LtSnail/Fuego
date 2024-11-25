# Client Application

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/SandboxApp.cpp)

set(DEPENDENCIES FuegoEngine)

if(APPLE)
  set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/MacOS/SandboxApp.mm)

  set(GLOBAL_RESOURCES Resources/MacOS/Assets.xcassets)

  set(BASE_RESOURCES Resources/MacOS/Base.lproj/Main.storyboard
                     ${ENGINE_SHADERS})
endif()

set(RESOURCES ${GLOBAL_RESOURCES} ${BASE_RESOURCES})

if(APPLE)
  add_executable(
    Sandbox MACOSX_BUNDLE ${SOURCES} ${PUBLIC_HEADERS} ${PROJECT_HEADERS}
                          ${PRIVATE_HEADERS} ${RESOURCES})

  set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION
                                                      Resources)

  set_property(TARGET Sandbox PROPERTY XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER
                                       Sandbox)

  set(MACOSX_BUNDLE_GUI_IDENTIFIER Sandbox)
  set(MACOSX_BUNDLE_BUNDLE_NAME ${ORGANIZATION}.Sandbox)

  set_target_properties(
    Sandbox
    PROPERTIES MACOSX_BUNDLE_INFO_PLIST
               ${CMAKE_CURRENT_SOURCE_DIR}/Resources/MacOS/Sandbox-Info.plist.in
  )
elseif(WIN32)
  add_executable(Sandbox WIN32 ${SOURCES})
endif()

target_link_libraries(Sandbox ${DEPENDENCIES})
