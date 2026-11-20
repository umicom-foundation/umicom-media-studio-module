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

# Load the dependency only when the parent build has not already provided its target.
# Explicit SDK selection supersedes ambient package lookup; retain the old
# lookup for review without letting another installation mask a missing SDK.
if(FALSE)
if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()
endif()

# An independent product selects one installed SDK explicitly. Parent builds
# keep their existing source targets and never silently mix package origins.
set(UMICOM_SDK_ROOT "" CACHE PATH "Selected installed Framework SDK prefix")
if(NOT TARGET Umicom::Framework)
    if(NOT IS_ABSOLUTE "${UMICOM_SDK_ROOT}" OR NOT IS_DIRECTORY "${UMICOM_SDK_ROOT}")
        message(FATAL_ERROR "Independent Umicom Media configuration requires an absolute existing UMICOM_SDK_ROOT")
    endif()
    # Offline inspection now uses the SDK component; preserve the earlier dependency list.
    if(FALSE)
    find_package(UmicomFramework 0.9 CONFIG REQUIRED COMPONENTS Framework application creative_workspace
        PATHS "${UMICOM_SDK_ROOT}" NO_DEFAULT_PATH)
    endif()
    find_package(UmicomFramework 0.9 CONFIG REQUIRED COMPONENTS Framework application creative_workspace comfy_protocol
        PATHS "${UMICOM_SDK_ROOT}" NO_DEFAULT_PATH)
endif()
get_target_property(_umicom_media_imported Umicom::Framework IMPORTED)
if(_umicom_media_imported)
    if(NOT COMMAND umicom_require_installed_sdk OR NOT COMMAND umicom_require_sdk_environment OR
       NOT COMMAND umicom_extend_thin_application_runtime)
        message(FATAL_ERROR "The selected Umicom SDK does not supply current application composition helpers")
    endif()
    # Offline inspection now uses the SDK component; preserve the earlier dependency list.
    if(FALSE)
    umicom_require_installed_sdk("${UMICOM_SDK_ROOT}" Umicom::Framework Umicom::application Umicom::creative_workspace)
    endif()
    umicom_require_installed_sdk("${UMICOM_SDK_ROOT}" Umicom::Framework Umicom::application Umicom::creative_workspace Umicom::comfy_protocol)
    umicom_require_sdk_environment(2)
elseif(UMICOM_SDK_ROOT)
    message(FATAL_ERROR "Umicom Media cannot combine source-built Framework targets with UMICOM_SDK_ROOT")
endif()


# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom Media Studio requires Framework target Umicom::application")
endif()

# Define the local fallback only when the parent build did not provide the shared helper.
if(NOT COMMAND umicom_apply_warnings)
    # Define the apply warnings build helper so parent and application projects apply one
    # consistent rule.
    function(umicom_apply_warnings target)
        # Select the warning options understood by the active compiler.
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        # Use this fallback path when the earlier condition does not apply.
        else()
            target_compile_options(${target} PRIVATE
                -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
        endif()
    endfunction()
endif()

# Define the local fallback only when the parent build did not provide the shared helper.
if(NOT COMMAND umicom_apply_sanitizers)
    # Define the apply sanitizers build helper so parent and application projects apply one
    # consistent rule.
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

# The Framework package or parent now publishes this helper without a source checkout.
# Preserve the former source-relative include for review.
if(FALSE)
include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomThinApplicationRuntime.cmake")
endif()
umicom_extend_thin_application_runtime(
    NAME "media"
    TARGET "umicom_media_studio_module")

# Create this optional product surface only when its build option is enabled.
if(UMICOM_MEDIA_STUDIO_BUILD_CONSOLE)
    add_executable(umicom-media-studio-console
        src/console/main.c)
    # Offline inspection now uses the SDK component; preserve the earlier dependency list.
    if(FALSE)
    target_link_libraries(umicom-media-studio-console PRIVATE Umicom::media_studio_module)
    endif()
    target_link_libraries(umicom-media-studio-console PRIVATE Umicom::media_studio_module Umicom::comfy_protocol Umicom::platform)
    # Progress-file inspection stays in Framework and is available from installed SDK exports.
    target_link_libraries(umicom-media-studio-console PRIVATE Umicom::ai)
    # Source review uses the same installed creative workspace as Media briefs.
    target_link_libraries(umicom-media-studio-console PRIVATE Umicom::creative_workspace)
    umicom_apply_warnings(umicom-media-studio-console)
    umicom_apply_sanitizers(umicom-media-studio-console)

    # Console products use the same Framework-owned identity as their GUI host.
    # Retain console subsystem and existing runtime ownership; this adds only
    # canonical artwork and native executable metadata.
    # Resource ownership moved to the package; retain previous source-only
    # wiring for review. SVG/ICO identity and console behavior are preserved.
    if(FALSE)
    if(NOT COMMAND umicom_apply_application_branding)
        include("${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationBranding.cmake")
    endif()
    if(WIN32)
        enable_language(RC)
    endif()
    umicom_apply_application_branding(
        TARGET umicom-media-studio-console
        PRODUCT_NAME "Umicom Media Console" INTERNAL_NAME "umicom-media-studio-console"
        VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
        VERSION_MINOR ${PROJECT_VERSION_MINOR}
        VERSION_PATCH ${PROJECT_VERSION_PATCH}
        RESOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/resources"
        NO_RUNTIME_DEPLOYMENT)
    endif()
    # Framework resolves its own source or installed SVG/ICO resources.
    if(NOT COMMAND umicom_apply_application_branding)
        message(FATAL_ERROR "Selected Framework must supply application branding")
    endif()
    if(WIN32)
        enable_language(RC)
    endif()
    umicom_apply_application_branding(
        TARGET umicom-media-studio-console
        PRODUCT_NAME "Umicom Media Console" INTERNAL_NAME "umicom-media-studio-console"
        VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
        VERSION_MINOR ${PROJECT_VERSION_MINOR}
        VERSION_PATCH ${PROJECT_VERSION_PATCH}
        NO_RUNTIME_DEPLOYMENT)
endif()

# Register verification targets only when the developer has enabled testing.
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
# Configure the optional target only when its feature has created it.
if(TARGET umicom-media-studio-console)
    install(TARGETS umicom-media-studio-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES application.umicom.yaml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/media)
# Use the optional file only when it is present in this checkout.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    install(FILES LICENSE
        DESTINATION ${CMAKE_INSTALL_DATADIR}/licenses/media)
endif()

message(STATUS "Umicom Media Studio Module version: ${PROJECT_VERSION}")
message(STATUS "Umicom Media Studio uses Framework-owned application experience")

# Product contribution composition is exported by the selected Framework SDK.
# Preserve the former source-relative include for review.
if(FALSE)
include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationProductisationContribution.cmake")
endif()
umicom_add_application_productisation_contribution(
    NAME "media"
    TARGET umicom_media_productisation
    SOURCE "src/productisation_contribution.c"
    TEST_SOURCE "tests/test_productisation_contribution.c")

# Compose the common creative document, animation, note and export services.
# Existing product controllers, consoles, tests and layouts remain active.
# Creative attachment uses the existing exported component; it must not rebuild Framework sources.
# Preserve the former source-relative include for review.
if(FALSE)
include("${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomCreativeWorkspace.cmake")
endif()
umicom_attach_creative_workspace(
    NAME "media"
    TARGET umicom_media_studio_module)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/LOCAL_ASSETS.html" DESTINATION "${CMAKE_INSTALL_DATADIR}/umicom/applications/media/docs")

# Install the user workflow alongside the other Media guides.
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/SEEDREAM_IMAGES.html" DESTINATION "${CMAKE_INSTALL_DATADIR}/umicom/applications/media/docs")

# Ship the shared creative workflow entry point with the application's help.
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/SONG_AND_VIDEO_WORKFLOWS.html"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/umicom/applications/media/docs")

# Keep the caption workflow available in the installed application help.
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/CAPTIONS.html"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/umicom/applications/media/docs")

# Ship the practical native-control guide with the existing product help.
install(FILES docs/NATIVE_WORKFLOWS.html
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/media/docs)

# These console tests use the SDK-owned fixture driver. When the native CLI is
# present, compare its output and exit status with the thin Media route too.
if(BUILD_TESTING AND TARGET umicom-media-studio-console)
    if(NOT COMMAND umicom_add_comfy_inspection_command_tests)
        message(FATAL_ERROR "Media console qualification requires the current Framework inspection helper")
    endif()
    if(TARGET umicom)
        umicom_add_comfy_inspection_command_tests(umicom-media-studio-console media.comfy.inspection COMPARE_TARGET umicom)
    else()
        umicom_add_comfy_inspection_command_tests(umicom-media-studio-console media.comfy.inspection)
    endif()
endif()

# Media calls the shared reader; run the same command fixtures as the native CLI.
if(BUILD_TESTING AND TARGET umicom-media-studio-console)
    if(NOT COMMAND umicom_add_pull_inspection_command_tests)
        message(FATAL_ERROR "Media qualification requires the Framework model-progress helper")
    endif()
    umicom_add_pull_inspection_command_tests(umicom-media-studio-console media.models.pull)
endif()


# Queue inspection is the existing Framework route; Media adds no parser.
if(BUILD_TESTING AND TARGET umicom-media-studio-console)
    if(NOT COMMAND umicom_add_comfy_queue_command_tests)
        message(FATAL_ERROR "Media qualification requires the Framework captured queue helper")
    endif()
    if(TARGET umicom)
        umicom_add_comfy_queue_command_tests(umicom-media-studio-console media.comfy.queue COMPARE_TARGET umicom)
    else()
        umicom_add_comfy_queue_command_tests(umicom-media-studio-console media.comfy.queue)
    endif()
endif()


# Device report behavior is shared; Media contributes no system-statistics parser.
if(BUILD_TESTING AND TARGET umicom-media-studio-console)
    if(NOT COMMAND umicom_add_comfy_system_command_tests)
        message(FATAL_ERROR "Media qualification requires the Framework captured system helper")
    endif()
    if(TARGET umicom)
        umicom_add_comfy_system_command_tests(umicom-media-studio-console media.comfy.system COMPARE_TARGET umicom)
    else()
        umicom_add_comfy_system_command_tests(umicom-media-studio-console media.comfy.system)
    endif()
endif()

# Media and CLI must publish identical observations for identical captured files.
if(BUILD_TESTING AND TARGET umicom-media-studio-console)
    if(NOT COMMAND umicom_add_comfy_node_pages_command_tests)
        message(FATAL_ERROR "Media node browsing requires the shared SDK qualification helper")
    endif()
    if(TARGET umicom)
        umicom_add_comfy_node_pages_command_tests(umicom-media-studio-console media.comfy.nodes COMPARE_TARGET umicom)
    else()
        umicom_add_comfy_node_pages_command_tests(umicom-media-studio-console media.comfy.nodes)
    endif()
endif()
