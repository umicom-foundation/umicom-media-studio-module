/*-----------------------------------------------------------------------------
 * Umicom Media Studio Module
 * File: src/console/main.c
 *
 * PURPOSE:
 *   Provide a native verification frontend for the thin product composition.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "umicom/media_generation/comfy_inspection.h"
#include "umicom/ai/ollama_pull_file.h"
#include "umicom/platform/native_arguments.h"
#if __has_include("umicom/creative_workspace/media_source_excerpt.h")
#include "umicom/creative_workspace/media_source_excerpt.h"
#endif

#include "umicom/media_studio/application.h"
#include "umicom/media_studio/workspace.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
/* Preserve the no-argument composition report while adding a thin shared Framework command route. The former entry-point signature is retained for review. The former implementation is retained for owner review. */
#if 0
int main(void)
#endif
static int MediaCompositionReport(void)
{
    UmiApplicationExperienceStatus status;
    const UmiApplicationExperienceDefinition *definition =
        umi_media_studio_application_experience();
    const UmiExperienceLayoutDefinition *layout =
        umi_media_studio_workspace_default();
    const UmiExperienceFeatureDefinition *next =
        umi_media_studio_workspace_next_feature();

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (definition == NULL ||
        umi_media_studio_application_status(&status) != UMI_STATUS_OK ||
        layout == NULL) {
        fputs("Umicom Media Studio: Framework experience unavailable\n", stderr);
        return 1;
    }

    printf("%s\n", definition->display_name);
    printf("application: %s\n", definition->application_id);
    printf("default layout: %s\n", layout->title);
    printf("panels: %zu\n", status.panel_count);
    printf("layouts: %zu\n", status.layout_count);
    printf("features: %zu\n", status.feature_count);
    printf("readiness: %u%%\n", status.readiness_percent);
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (next != NULL)
        printf("next: %s [%s]\n", next->title,
               umi_experience_priority_text(next->priority));
    return 0;
}

/* Media owns only presentation routing. Settings, filesystem admission,
 * workflow inspection and JSON serialization stay in Framework. */
static int MediaCommand(int argc, char **argv)
{
    if (argc < 2) return MediaCompositionReport();
    /* This route admits only offline progress inspection, not model execution. */
    if (strcmp(argv[1], "models") == 0)
        return UmiOllamaPullFileCommand(argc - 2, argv + 2);
    if (strcmp(argv[1], "comfy") == 0)
        return UmiComfyInspectionCommand(argc - 2, argv + 2);
    /* Send exact source review and extraction to Framework. Installed
     * SDKs without the checked reader can still run the other routes. */
    if (strcmp(argv[1], "source") == 0) {
#if defined(UMI_MEDIA_SOURCE_EXCERPT_INSPECTION)
        return UmiMediaSourceExcerptCommand(argc - 2, argv + 2);
#else
        fputs("The selected Framework SDK does not supply source excerpt inspection.\n", stderr);
        return 1;
#endif
    }
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "help") == 0) {
        fputs("Umicom Media Console\nNo arguments: product composition report\n"
              "comfy inspect CONNECTION WORKFLOW OBJECT_INFO [--client-id ID]\n"
              "comfy references --help: paged candidate observations\n"
              "comfy queue --help: captured running/pending observations\n"
              "comfy system --help: captured system/device observations\n"
              "comfy nodes --help: searchable captured node catalogue\n"
              "comfy --help: offline inspection help\n"
              "source --help: inspect and repeat a local UTF-8 selection\n"
              "models inspect-pull ABSOLUTE_NDJSON: captured model-progress metadata\n", stdout);
        return ferror(stdout) ? 1 : 0;
    }
    fputs("Umicom Media Console: unknown command; use --help\n", stderr);
    return 2;
}
int main(int argc, char **argv)
{
    int exit_code = 1;
    /* Windows command-line text is decoded by the same UTF-8 service used by
     * the native CLI; filesystem paths are never locale-dependent byte guesses. */
    UmiStatus status = UmiNativeArgumentsDispatch(argc, argv, MediaCommand, &exit_code);
    if (status != UMI_STATUS_OK) {
        fprintf(stderr, "Umicom Media Console: %s\n", umi_status_text(status));
        return 1;
    }
    return exit_code;
}
