cmake_minimum_required(VERSION 3.15)
project(geeTetris)

include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)

set(SOURCES
  src/shared/casioCalcs.c
  src/shared/menu.c
  src/shared/keys.c
  src/shared/window.c
  src/tetrisParameters.c
  src/scores.c
  src/piece.c
  src/playArea.c
  src/tetrisGame.c
  src/geeTetris.c
)
set(ASSETS
  assets/font_horz.png
  assets/font_vert.png
 )
set(ASSETS_cg
  assets-cg/coloured.png
  assets-cg/pause.png
  assets-cg/menu.png
)

fxconv_declare_assets(${ASSETS} ${ASSETS_cg} WITH_METADATA)

add_executable(geeTetris ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(geeTetris PRIVATE -Wall -Wextra -Os -D=DEST_CASIO_CALC -D=NO_CAPTURE)
target_link_libraries(geeTetris Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET geeTetris OUTPUT "geeTetris.g3a"
    NAME "geeTetris" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
