/*-----------------------------------------------------------------------------
 * Umicom Media Studio Module
 * File: src/productisation_contribution.c
 *
 * PURPOSE:
 *   Bind product identity and executable evidence to canonical Framework
 *   application definitions without copying shared implementation.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#include "umicom/media/productisation_contribution.h"

static const UmiProductApplicationAdoption ADOPTION = {
    sizeof(UmiProductApplicationAdoption),
    "media",
    "org.umicom.media-studio",
    "Umicom Media Studio",
    "umicom-media-console",
    UMI_PRODUCT_FRONTEND_CONSOLE,
    1,
    1,
    1,
    1
};

const UmiProductApplicationAdoption *
umi_media_productisation_contribution(void)
{
    return &ADOPTION;
}

UmiStatus umi_media_productisation_snapshot(
    UmiProductApplicationAdoptionSnapshot *out_snapshot)
{
    return umi_product_application_adoption_snapshot(
        &ADOPTION, out_snapshot);
}
