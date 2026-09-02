/*-----------------------------------------------------------------------------
 * Umicom Media Studio Module
 * File: src/readiness.c
 *
 * PURPOSE:
 *   Project the canonical Framework feature backlog without product-local roadmap duplication.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/


#include "umicom/media_studio/readiness.h"

#include "umicom/media_studio/runtime.h"
#include "umicom/application/experience_plan.h"

/*
 * Provide the media studio readiness report operation used by this module and its client
 * applications.
 */
UmiStatus umi_media_studio_readiness_report(
    UmiApplicationReadinessReport *out_report)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_media_studio_runtime_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_readiness_report(experience, out_report);
}

/*
 * Provide the media studio readiness next feature operation used by this module and its
 * client applications.
 */
const UmiExperienceFeatureDefinition *umi_media_studio_readiness_next_feature(void)
{
    return umi_application_experience_next_feature(
        umi_media_studio_runtime_experience());
}
