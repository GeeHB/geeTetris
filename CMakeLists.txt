cmake_minimum_required(VERSION 3.15)
project(geeTetris)

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)

set(SOURCES
  src/geeTetris.cpp
  src/tetrisMenu.cpp
  src/shared/scrCapture.cpp
  src/shared/keyboard.cpp
  src/shared/bFile.cpp
  src/shared/menuBar.cpp
  src/shared/window.cpp
  src/tetrisGame.cpp
  src/piece.cpp
  src/playArea.cpp
  src/sList.cpp
)
# Shared assets, fx-9860G-only assets and fx-CG-50-only assets
set(ASSETS
  assets/font_horz.png
  assets/font_vert.png
 )
 set(ASSETS_fx
  assets-fx/bw.png
  #assets-fx/pause.png
)
set(ASSETS_cg
  assets-cg/coloured.png
  assets-cg/pause.png
  assets-cg/menu.png
)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(geeTetris ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(geeTetris PRIVATE -Wall -Wextra -Os -D=DEST_CASIO_CALC -D=NO_CAPTURE)
target_link_libraries(geeTetris Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET geeTetris OUTPUT "geeTetris.g1a"
    NAME "geeTetris" ICON assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET geeTetris OUTPUT "geeTetris.g3a"
    NAME "geeTetris" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
