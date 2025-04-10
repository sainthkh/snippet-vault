cmake_minimum_required(VERSION 3.16)

project(SnippetVault VERSION 1.0)

include(FetchContent)

#
# Fetch Contents
#
############################################

FetchContent_Declare(
    Utest
    GIT_REPOSITORY https://github.com/sainthkh/utest.h.git
    GIT_TAG 03d84c27426ff042caaf6d668ffafad89502846e
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(Utest)

FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.2.8
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG release-3.2.4
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
    GIT_TAG release-3.2.0
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL SDL_image SDL_ttf)

FetchContent_Declare(
    quickjs
    GIT_REPOSITORY https://github.com/quickjs-ng/quickjs.git
    GIT_TAG v0.9.0
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(quickjs)

#
# SnippetVaultLib
#
#################################

add_library(SnippetVaultLib
  src/add.c
  src/js.c
  src/stringstore.c
)

target_include_directories(SnippetVaultLib PRIVATE
    build/_deps/quickjs-src
)

target_link_libraries(SnippetVaultLib qjs)

#
# SnippetVault
#
##################################

add_executable(SnippetVault
  src/api.c
  src/main.c
)

target_include_directories(SnippetVault PRIVATE
    build/_deps/quickjs-src
)

target_link_libraries(SnippetVault SnippetVaultLib)
target_link_libraries(SnippetVault SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
target_link_libraries(SnippetVault qjs)

add_custom_command(TARGET SnippetVault POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SnippetVault> ${CMAKE_BINARY_DIR}
)

# Copy SDL dlls to build root
# TODO: find better way to do this
add_custom_command(TARGET SnippetVault POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/_deps/sdl-build/SDL3.dll ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET SnippetVault POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/_deps/sdl_image-build/SDL3_image.dll ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET SnippetVault POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/_deps/sdl_ttf-build/SDL3_ttf.dll ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET SnippetVault POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/_deps/quickjs-build/libqjs.dll ${CMAKE_BINARY_DIR}
)

#
# SnippetVaultTest
#
###################################

add_executable(SnippetVaultTest
  src/main.test.c
  src/add.test.c
  src/js.test.c
  src/stringstore.test.c
)

target_include_directories(SnippetVaultTest PRIVATE src)
target_link_libraries(SnippetVaultTest SnippetVaultLib Utest)
add_test(NAME SnippetVaultTest COMMAND SnippetVaultTest)
