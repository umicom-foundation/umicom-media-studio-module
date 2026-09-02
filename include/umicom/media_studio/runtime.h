/*-----------------------------------------------------------------------------
 * Umicom Media Studio Module
 * File: include/umicom/media_studio/runtime.h
 *
 * PURPOSE:
 *   Bind the thin product to the Framework-owned application workspace runtime without duplicating services.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_MEDIA_STUDIO_RUNTIME_H
#define UMICOM_MEDIA_STUDIO_RUNTIME_H

#include "umicom/application/runtime/runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialise media studio runtime from caller-provided values so later operations receive
 * a known state.
 */
UmiStatus umi_media_studio_runtime_init(
    UmiApplicationWorkspaceRuntime *out_runtime);
/**
 * Provide the media studio runtime health operation used by this module and its client
 * applications.
 */
UmiStatus umi_media_studio_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health);
/**
 * Provide the media studio runtime experience operation used by this module and its client
 * applications.
 */
const UmiApplicationExperienceDefinition *umi_media_studio_runtime_experience(void);

#ifdef __cplusplus
}
#endif

#endif
