#-----------------------------------------------------------------------------
# Umicom Media Studio Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build a thin C23 product composition over Framework-owned application
#   experience, runtime and workbench contracts.
#
# AUTHOR AND ORGANISATION:
#   Sammy Hegab
#   Umicom Foundation
#
# LICENCE:
#   MIT
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.24)

project(
    MediaStudioModule
    VERSION 0.1.0
    DESCRIPTION "Thin Umicom Media Studio composition over Umicom Framework"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(UMICOM_MEDIA_STUDIO_BUILD_CONSOLE
       "Build the native Umicom Media Studio verification console"
       ON)

include(CTest)
include(GNUInstallDirs)

if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()

if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom Media Studio requires Framework target Umicom::application")
endif()

if(NOT COMMAND umicom_apply_warnings)
    function(umicom_apply_warnings target)
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        else()
            target_compile_options(${target} PRIVATE
                -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
        endif()
    endfunction()
endif()

if(NOT COMMAND umicom_apply_sanitizers)
    function(umicom_apply_sanitizers target)
    endfunction()
endif()

add_library(umicom_media_studio_module STATIC
    src/application.c
    src/workspace.c
)
add_library(Umicom::media_studio_module ALIAS umicom_media_studio_module)

target_include_directories(umicom_media_studio_module PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(umicom_media_studio_module PUBLIC
    Umicom::Framework
    Umicom::application
)
umicom_apply_warnings(umicom_media_studio_module)
umicom_apply_sanitizers(umicom_media_studio_module)

include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomThinApplicationRuntime.cmake")
umicom_extend_thin_application_runtime(
    NAME "media"
    TARGET "umicom_media_studio_module")

if(UMICOM_MEDIA_STUDIO_BUILD_CONSOLE)
    add_executable(umicom-media-studio-console
        src/console/main.c)
    target_link_libraries(umicom-media-studio-console PRIVATE Umicom::media_studio_module)
    umicom_apply_warnings(umicom-media-studio-console)
    umicom_apply_sanitizers(umicom-media-studio-console)
endif()

if(BUILD_TESTING)
    add_executable(media-application-test
        tests/test_application.c)
    target_link_libraries(media-application-test PRIVATE Umicom::media_studio_module)
    umicom_apply_warnings(media-application-test)
    umicom_apply_sanitizers(media-application-test)
    add_test(NAME media.module.application
             COMMAND media-application-test)

    add_executable(media-workspace-test
        tests/test_workspace.c)
    target_link_libraries(media-workspace-test PRIVATE Umicom::media_studio_module)
    umicom_apply_warnings(media-workspace-test)
    umicom_apply_sanitizers(media-workspace-test)
    add_test(NAME media.module.workspace
             COMMAND media-workspace-test)
endif()

install(TARGETS umicom_media_studio_module
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
if(TARGET umicom-media-studio-console)
    install(TARGETS umicom-media-studio-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES application.umicom.yaml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/media)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    install(FILES LICENSE
        DESTINATION ${CMAKE_INSTALL_DATADIR}/licenses/media)
endif()

message(STATUS "Umicom Media Studio Module version: ${PROJECT_VERSION}")
message(STATUS "Umicom Media Studio uses Framework-owned application experience")

include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationProductisationContribution.cmake")
umicom_add_application_productisation_contribution(
    NAME "media"
    TARGET umicom_media_productisation
    SOURCE "src/productisation_contribution.c"
    TEST_SOURCE "tests/test_productisation_contribution.c")
