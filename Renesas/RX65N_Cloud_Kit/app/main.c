/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include <stdio.h>

#include "tx_api.h"

#include "board_init.h"
#include "rx_networking.h"
#include "sntp_client.h"

#include "legacy/mqtt.h"
#include "nx_client.h"

#include "azure_config.h"

#include "rx65n_cloud_kit_sensors.h"

#define AZURE_THREAD_STACK_SIZE 4096
#define AZURE_THREAD_PRIORITY   5

TX_THREAD azure_thread;
ULONG azure_thread_stack[AZURE_THREAD_STACK_SIZE / sizeof(ULONG)];

void azure_thread_entry(ULONG parameter);
void tx_application_define(void* first_unused_memory);

void azure_thread_entry(ULONG parameter)
{
    UINT status;

    printf("\r\nStarting Azure thread\r\n\r\n");

    // Initialize the network
    if (rx_network_init(WIFI_SSID, WIFI_PASSWORD, WIFI_MODE) != NX_SUCCESS)
    {
        printf("Failed to initialize the network\r\n");
        return;
    }

    // Start the SNTP client
    status = sntp_start();
    if (status != NX_SUCCESS)
    {
        printf("Failed to start the SNTP client (0x%02x)\r\n", status);
        return;
    }

    // Wait for an SNTP sync
    status = sntp_sync_wait();
    if (status != NX_SUCCESS)
    {
        printf("Failed to start sync SNTP time (0x%02x)\r\n", status);
        return;
    }

    // Stop the SNTP thread, the RX65N cloud wifi driver only works with a single socket at once
    sntp_stop();

#ifdef ENABLE_LEGACY_MQTT
    if ((status = azure_iot_mqtt_entry(&nx_ip, &nx_pool, &nx_dns_client, sntp_time_get)))
#else
    if ((status = azure_iot_nx_client_entry(&nx_ip, &nx_pool, &nx_dns_client, sntp_time)))
#endif
    {
        printf("Failed to run Azure IoT (0x%04x)\r\n", status);
        return;
    }
}

void tx_application_define(void* first_unused_memory)
{
    // Create Azure SDK thread.
    UINT status = tx_thread_create(&azure_thread,
        "Azure Thread",
        azure_thread_entry,
        0,
        azure_thread_stack,
        AZURE_THREAD_STACK_SIZE,
        AZURE_THREAD_PRIORITY,
        AZURE_THREAD_PRIORITY,
        TX_NO_TIME_SLICE,
        TX_AUTO_START);

    if (status != TX_SUCCESS)
    {
        printf("Azure IoT application failed, please restart\r\n");
    }
}

int main(void)
{
    // Initialise the board
    board_init();

    tx_kernel_enter();

    return 0;
}
