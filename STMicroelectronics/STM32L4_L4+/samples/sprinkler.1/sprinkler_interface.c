// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.

/********************************************************************
 * This code was auto-generated from sprinkler.1 DL and shall not be
 * modified.
 ********************************************************************/

#include "az_ulib_capability_api.h"
#include "az_ulib_descriptor_api.h"
#include "az_ulib_ipc_interface.h"
#include "az_ulib_result.h"
#include "azure/az_core.h"
#include "sprinkler.h"
#include "sprinkler_1_model.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static az_result sprinkler_1_water_now_concrete(
    const sprinkler_1_water_now_model_in* const in,
    az_ulib_model_out out)
{
  (void)out;
  return sprinkler_v1i1_water_now(in->zone, in->timer);
}

static az_result sprinkler_1_water_now_span_wrapper(az_span model_in_span, az_span* model_out_span)
{
  AZ_ULIB_TRY
  {
    // Unmarshalling JSON in model_in_span to water_now_model_in.
    az_json_reader jr;
    sprinkler_1_water_now_model_in water_now_model_in = { 0 };
    AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_init(&jr, model_in_span, NULL));
    AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
    while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
    {
      if (az_json_token_is_text_equal(&jr.token, AZ_SPAN_FROM_STR(SPRINKLER_1_ZONE_NAME)))
      {
        AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
        AZ_ULIB_THROW_IF_AZ_ERROR(az_span_atoi32(jr.token.slice, &(water_now_model_in.zone)));
      }
      if (az_json_token_is_text_equal(&jr.token, AZ_SPAN_FROM_STR(SPRINKLER_1_TIMER_NAME)))
      {
        AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
        AZ_ULIB_THROW_IF_AZ_ERROR(az_span_atoi32(jr.token.slice, &(water_now_model_in.timer)));
      }
      AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
    }
    AZ_ULIB_THROW_IF_AZ_ERROR(AZ_ULIB_TRY_RESULT);

    // Call.
    AZ_ULIB_THROW_IF_AZ_ERROR(
        sprinkler_1_water_now_concrete(&water_now_model_in, NULL));

    // Marshalling empty water_now_model_out to JSON in model_out_span.
    *model_out_span = az_span_create_from_str("{}");
  }
  AZ_ULIB_CATCH(...) {}

  return AZ_ULIB_TRY_RESULT;
}

static az_result sprinkler_1_stop_concrete(
    const sprinkler_1_stop_model_in* const in,
    az_ulib_model_out out)
{
  (void)out;
  return sprinkler_v1i1_stop(*in);
}

static az_result sprinkler_1_stop_span_wrapper(az_span model_in_span, az_span* model_out_span)
{
  AZ_ULIB_TRY
  {
    // Unmarshalling empty JSON in model_in_span to stop_model_in.
    az_json_reader jr;
    sprinkler_1_stop_model_in stop_model_in = { 0 };
    AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_init(&jr, model_in_span, NULL));
    AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
    AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
    while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
    {
      if (az_json_token_is_text_equal(&jr.token, AZ_SPAN_FROM_STR(SPRINKLER_1_ZONE_NAME)))
      {
        AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
        AZ_ULIB_THROW_IF_AZ_ERROR(az_span_atoi32(jr.token.slice, &(stop_model_in)));
      }
      AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
    }
    AZ_ULIB_THROW_IF_AZ_ERROR(AZ_ULIB_TRY_RESULT);

    // Call.
    AZ_ULIB_THROW_IF_AZ_ERROR(sprinkler_1_stop_concrete(&stop_model_in, NULL));

    // Marshalling empty stop_model_out to JSON in model_out_span.
    *model_out_span = az_span_create_from_str("{}");
  }
  AZ_ULIB_CATCH(...) {}

  return AZ_ULIB_TRY_RESULT;
}

static const az_ulib_capability_descriptor SPRINKLER_1_CAPABILITIES[]
    = { AZ_ULIB_DESCRIPTOR_ADD_CAPABILITY(
            SPRINKLER_1_WATER_NOW_COMMAND_NAME,
            sprinkler_1_water_now_concrete,
            sprinkler_1_water_now_span_wrapper),
        AZ_ULIB_DESCRIPTOR_ADD_CAPABILITY(
            SPRINKLER_1_STOP_COMMAND_NAME,
            sprinkler_1_stop_concrete,
            sprinkler_1_stop_span_wrapper) };

static const az_ulib_interface_descriptor SPRINKLER_1_DESCRIPTOR = AZ_ULIB_DESCRIPTOR_CREATE(
    SPRINKLER_1_PACKAGE_NAME,
    SPRINKLER_1_PACKAGE_VERSION,
    SPRINKLER_1_INTERFACE_NAME,
    SPRINKLER_1_INTERFACE_VERSION,
    SPRINKLER_1_CAPABILITIES);

az_result publish_interface(const az_ulib_ipc_table* const table)
{
  return table->publish(&SPRINKLER_1_DESCRIPTOR, NULL);
}

az_result unpublish_interface(const az_ulib_ipc_table* const table)
{
  /**
   * @note: It is the user's responsibility to halt any processes that should not persist
   *        beyond package uninstall.
   */
  AZ_ULIB_TRY
  {
    // end sprinkler operations
    AZ_ULIB_THROW_IF_AZ_ERROR(sprinkler_v1i1_end());

    // unpublish interface
    AZ_ULIB_THROW_IF_AZ_ERROR(table->unpublish(&SPRINKLER_1_DESCRIPTOR, AZ_ULIB_NO_WAIT));
  }
  AZ_ULIB_CATCH(...) {}

  return AZ_ULIB_TRY_RESULT;
}
