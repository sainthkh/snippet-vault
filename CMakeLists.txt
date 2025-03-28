cmake_minimum_required(VERSION 3.16)

project(SnippetVault VERSION 1.0)

include(FetchContent)

FetchContent_Declare(
    Utest
    GIT_REPOSITORY https://github.com/sainthkh/utest.h.git
    GIT_TAG 03d84c27426ff042caaf6d668ffafad89502846e
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(Utest)

add_library(SnippetVaultLib
  src/add.c
)

#
# SnippetVault
#
##################################

add_executable(SnippetVault
  src/main.c
)

target_link_libraries(SnippetVault SnippetVaultLib)


#
# SnippetVaultTest
#
###################################

add_executable(SnippetVaultTest
  test/main.c
  test/add.test.c
)

target_include_directories(SnippetVaultTest PRIVATE src)
target_link_libraries(SnippetVaultTest SnippetVaultLib Utest)
add_test(NAME SnippetVaultTest COMMAND SnippetVaultTest)

