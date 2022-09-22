/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <string.h>

#include "port_common.h"
#include "mqtt_transport_interface.h"
#include "ssl_transport_interface.h"
#include "timer_interface.h"
#include "timer.h"

#include "../libraries/TLC_Libs/mqtt_certificate.h"

/* Port */
#define TARGET_PORT 8883

/* MQTT */
#define MQTT_PUB_PERIOD (1000 * 10) // 10 seconds

/* AWS IoT */
#define MQTT_DOMAIN "replace by your aws_iot_endpoint"
#define MQTT_PUB_TOPIC "tlc/pump/value"
#define MQTT_SUB_TOPIC "tlc/pump/status"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_CLIENT_ID "tlc-pump-c934975b-9be1-4b44-9b25-fe880bfd686e"

// Topic command to turn on pump
const char *pumpOn = "On";
// Topic command to turn off pump
const char *pumpOff = "Off";
const char *pumpStatus = "Get";

// GPIO2 Relay control pin
const uint RELAY_PIN = 22;
// Control pump On for status message
bool ispumpOn = false;

char mqtt_topic[30];
char mqtt_topic_value[10];

extern void put_pixel(uint32_t pixel_grb);
extern uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */

/* MQTT */
static uint8_t g_mqtt_buf[MQTT_BUF_MAX_SIZE] = {
    0,
};
static uint8_t g_mqtt_pub_msg_buf[MQTT_BUF_MAX_SIZE] = {
    0,
};

/* SSL */
tlsContext_t g_mqtt_tls_context;

void mqtt_event_callback2(MQTTContext_t *pContext, MQTTPacketInfo_t *pPacketInfo, MQTTDeserializedInfo_t *pDeserializedInfo)
{
    /* Handle incoming publish. The lower 4 bits of the publish packet
     * type is used for the dup, QoS, and retain flags. Hence masking
     * out the lower bits to check if the packet is publish. */
    if ((pPacketInfo->type & 0xF0U) == MQTT_PACKET_TYPE_PUBLISH)
    {
        /* Handle incoming publish. */
        if (pDeserializedInfo->pPublishInfo->payloadLength)
        {
            printf("%.*s,%d,%.*s\n", pDeserializedInfo->pPublishInfo->topicNameLength, pDeserializedInfo->pPublishInfo->pTopicName,
                   pDeserializedInfo->pPublishInfo->payloadLength,
                   pDeserializedInfo->pPublishInfo->payloadLength, pDeserializedInfo->pPublishInfo->pPayload);

            sprintf(mqtt_topic, "%.*s", pDeserializedInfo->pPublishInfo->topicNameLength, pDeserializedInfo->pPublishInfo->pTopicName);
            sprintf(mqtt_topic_value, "%.*s", pDeserializedInfo->pPublishInfo->payloadLength, pDeserializedInfo->pPublishInfo->pPayload);
            //put_pixel(urgb_u32(0, 0xff, 0xff)); // White
            if (strcmp(mqtt_topic, MQTT_SUB_TOPIC) == 0)
            {
                if (strcmp(mqtt_topic_value, pumpOn) == 0)
                {
                    put_pixel(urgb_u32(0xff, 0, 0)); // Red
                    ispumpOn = true;
                    gpio_put(RELAY_PIN, 1);
                    sleep_ms(2000);
                    
                }
                else if (strcmp(mqtt_topic_value, pumpOff) == 0)
                {
                    put_pixel(urgb_u32(0, 0xff, 0)); // Green
                    ispumpOn = false;
                    gpio_put(RELAY_PIN, 0);
                    sleep_ms(2000);
                    
                }
                else if (strcmp(mqtt_topic_value, pumpStatus) == 0)
                {
                    put_pixel(urgb_u32(0x99, 0, 0xcc)); // Magenta
                    sprintf(g_mqtt_pub_msg_buf, ispumpOn ? pumpOn : pumpOff);
                    mqtt_transport_publish(MQTT_PUB_TOPIC, g_mqtt_pub_msg_buf, strlen(g_mqtt_pub_msg_buf), 0);
                    sleep_ms(2000);
                }
            }
            put_pixel(urgb_u32(0, 0, 0xff)); // Blue
        }
    }
    else
    {
        /* Handle other packets. */
        switch (pPacketInfo->type)
        {
        case MQTT_PACKET_TYPE_SUBACK:
        {
            printf("TLC Pump Received SUBACK: PacketID=%u\n", pDeserializedInfo->packetIdentifier);
            break;
        }

        case MQTT_PACKET_TYPE_PINGRESP:
        {
            /* Nothing to be done from application as library handles
             * PINGRESP. */
            printf("Received PINGRESP\n");

            break;
        }

        case MQTT_PACKET_TYPE_UNSUBACK:
        {
            printf("Received UNSUBACK: PacketID=%u\n", pDeserializedInfo->packetIdentifier);

            break;
        }

        case MQTT_PACKET_TYPE_PUBACK:
        {
            printf("Received PUBACK: PacketID=%u\n", pDeserializedInfo->packetIdentifier);

            break;
        }

        case MQTT_PACKET_TYPE_PUBREC:
        {
            printf("Received PUBREC: PacketID=%u\n", pDeserializedInfo->packetIdentifier);

            break;
        }

        case MQTT_PACKET_TYPE_PUBREL:
        {
            /* Nothing to be done from application as library handles
             * PUBREL. */
            printf("Received PUBREL: PacketID=%u\n", pDeserializedInfo->packetIdentifier);

            break;
        }

        case MQTT_PACKET_TYPE_PUBCOMP:
        {
            /* Nothing to be done from application as library handles
             * PUBCOMP. */
            printf("Received PUBCOMP: PacketID=%u\n", pDeserializedInfo->packetIdentifier);

            break;
        }

        /* Any other packet type is invalid. */
        default:
        {
            printf("Unknown packet type received:(%02x).\n", pPacketInfo->type);
        }
        }
    }
}

/**
 * ----------------------------------------------------------------------------------------------------
 * demo
 * ----------------------------------------------------------------------------------------------------
 */
int mqtt_thing(void)
{
    int retval = 0;
    /* Setup certificate */
    g_mqtt_tls_context.rootca_option = MBEDTLS_SSL_VERIFY_REQUIRED; // use Root CA verify
    g_mqtt_tls_context.clica_option = 1;                            // use client certificate
    g_mqtt_tls_context.root_ca = mqtt_root_ca;
    g_mqtt_tls_context.client_cert = mqtt_client_cert;
    g_mqtt_tls_context.private_key = mqtt_private_key;
    uint32_t pub_cnt = 0;

    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);

    wizchip_1ms_timer_initialize(repeating_timer_callback);
    retval = mqtt_transport_init(true, MQTT_CLIENT_ID, NULL, NULL, MQTT_DEFAULT_KEEP_ALIVE);

    if (retval != 0)
    {
        printf(" Failed, mqtt_transport_init returned %d\n", retval);
        put_pixel(urgb_u32(0xff, 0x80, 0)); // Orange
        while (1)
            ;
    }

    retval = mqtt_transport_connect(1, g_mqtt_buf, MQTT_BUF_MAX_SIZE, MQTT_DOMAIN, TARGET_PORT, &g_mqtt_tls_context, mqtt_event_callback2);

    if (retval != 0)
    {
        printf(" Failed, mqtt_transport_connect returned %d\n", retval);
        put_pixel(urgb_u32(0xff, 0x80, 0)); // Orange
        while (1)
            ;
    }
    retval = mqtt_transport_subscribe(0, MQTT_SUB_TOPIC);

    if (retval != 0)
    {
        printf(" Failed, mqtt_transport_subscribe returned %d\n", retval);
        put_pixel(urgb_u32(0xff, 0x80, 0)); // Orange
        while (1)
            ;
    }

    printf(" AWS IoT connected\r\n");
    put_pixel(urgb_u32(0, 0, 0xff)); // Blue
    while (1)
    {
        retval = mqtt_transport_yield(MQTT_DEFAULT_YIELD_TIMEOUT);

        if (retval != 0)
        {
            printf(" Failed, mqtt_transport_yield returned %d\n", retval);
            put_pixel(0); // Turn Off
            while (1)
                ;
        }
        // sprintf(g_mqtt_pub_msg_buf, "{\"message\":\"Hello, World!\", \"publish count\":\"%d\"}\n", pub_cnt++);
        // mqtt_transport_publish(MQTT_PUB_TOPIC, g_mqtt_pub_msg_buf, strlen(g_mqtt_pub_msg_buf), 0);
    }
}

/*-----------------------------------------------------------*/
