// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.

/********************************************************************
 * This code was auto-generated from sprinkler v2 DL.
 *
 * Implement the code under the concrete functions.
 *
 ********************************************************************/

#include "az_ulib_capability_api.h"
#include "az_ulib_descriptor_api.h"
#include "az_ulib_ipc_api.h"
#include "az_ulib_result.h"
#include "sprinkler_v1i1.h"
#include "sprinkler_v1i1_interface.h"
#include "sprinkler_1_model.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static az_result sprinkler_1_call_w_str(az_span name, az_span model_in, az_span* model_out);

/*
 * Concrete implementations of the sprinkler 1 water_now commands.
 */
static az_result sprinkler_1_water_now_concrete(az_ulib_model_in model_in, az_ulib_model_out model_out)
{
  (void)model_out;
  const sprinkler_1_water_now_model_in* const in = (const sprinkler_1_water_now_model_in* const)model_in;
  return sprinkler_v1i1_water_now(in->timer);
}

/*
 * Concrete implementations of the sprinkler 1 stop commands.
 */
static az_result sprinkler_1_stop_concrete(az_ulib_model_in model_in, az_ulib_model_out model_out)
{
  (void)model_in;
  (void)model_out;
  return sprinkler_v1i1_stop();
}

static az_result sprinkler_1_water_now_span_wrapper(az_span model_in_span, az_span* model_out_span)
{
  az_result result = AZ_OK;

  AZ_ULIB_TRY
  {
    // Unmarshalling JSON to model_in_span.
    az_json_reader jr;
    sprinkler_1_water_now_model_in water_now_model_in = { 0 };
    AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_init(&jr, model_in_span, NULL));
    AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
    while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
    {
      if (az_json_token_is_text_equal(&jr.token, AZ_SPAN_FROM_STR(SPRINKLER_1_TIMER_NAME)))
      {
        AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
        AZ_ULIB_THROW_IF_AZ_ERROR(az_span_atoi32(jr.token.slice, &(water_now_model_in.timer)));
      }
      AZ_ULIB_THROW_IF_AZ_ERROR(az_json_reader_next_token(&jr));
    }
    AZ_ULIB_THROW_IF_AZ_ERROR(AZ_ULIB_TRY_RESULT);

    // Call.
    result = sprinkler_1_water_now_concrete((az_ulib_model_in)&water_now_model_in, NULL);

    // Marshalling model_out to JSON.
    *model_out_span = az_span_create_from_str("{}");
  }
  AZ_ULIB_CATCH(...) { result = AZ_ULIB_TRY_RESULT; }

  return result;
}

static az_result sprinkler_1_stop_span_wrapper(az_span model_in_span, az_span* model_out_span)
{
  az_result result = AZ_OK;

  AZ_ULIB_TRY
  {
    // Unmarshalling JSON to model_in.
    (void)model_in_span;

    // Call.
    result = sprinkler_1_stop_concrete(NULL, NULL);

    // Marshalling model_out to JSON.
    *model_out_span = az_span_create_from_str("{}");
  }
  AZ_ULIB_CATCH(...) { result = AZ_ULIB_TRY_RESULT; }

  return result;
}

static const az_ulib_capability_descriptor SPRINKLER_1_CAPABILITIES[SPRINKLER_1_CAPABILITY_SIZE] = {
  AZ_ULIB_DESCRIPTOR_ADD_COMMAND(
      SPRINKLER_1_WATER_NOW_COMMAND_NAME,
      sprinkler_1_water_now_concrete),
  AZ_ULIB_DESCRIPTOR_ADD_COMMAND(SPRINKLER_1_STOP_COMMAND_NAME, sprinkler_1_stop_concrete)
};

static const az_ulib_interface_descriptor SPRINKLER_1_DESCRIPTOR = AZ_ULIB_DESCRIPTOR_CREATE(
    SPRINKLER_1_INTERFACE_NAME,
    SPRINKLER_1_INTERFACE_VERSION,
    SPRINKLER_1_CAPABILITY_SIZE,
    sprinkler_1_call_w_str,
    SPRINKLER_1_CAPABILITIES);

static az_result sprinkler_1_call_w_str(az_span name, az_span model_in_span, az_span* model_out_span)
{
  uint8_t index;
  az_result result;

  for (index = 0; index < SPRINKLER_1_DESCRIPTOR.size; index++)
  {
    if (az_span_is_content_equal(name, SPRINKLER_1_DESCRIPTOR.capability_list[index].name))
    {
      break;
    }
  }

  switch (index)
  {
    case SPRINKLER_1_WATER_NOW_COMMAND:
      result = sprinkler_1_water_now_span_wrapper(model_in_span, model_out_span);
      break;
    case SPRINKLER_1_STOP_COMMAND:
    {
      result = sprinkler_1_stop_span_wrapper(model_in_span, model_out_span);
      break;
    }
    default:
      result = AZ_ERROR_ITEM_NOT_FOUND;
      break;
  }

  return result;
}

az_result sprinkler_v1i1_interface_publish(void)
{
  return az_ulib_ipc_publish(&SPRINKLER_1_DESCRIPTOR, NULL);
}

az_result sprinkler_v1i1_interface_unpublish(void)
{
#ifdef AZ_ULIB_CONFIG_IPC_UNPUBLISH
  return az_ulib_ipc_unpublish(&SPRINKLER_1_DESCRIPTOR, AZ_ULIB_NO_WAIT);
#else
  return AZ_OK;
#endif // AZ_ULIB_CONFIG_IPC_UNPUBLISH
}
