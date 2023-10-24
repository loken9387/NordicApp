#include "network_client.h"
#include <net/mqtt.h>
#include <logging/log.h>
LOG_MODULE_REGISTER(network_client, LOG_LEVEL_DBG);

static struct mqtt_client client;
static uint8_t rx_buffer[256];  // Adjust size as needed
static uint8_t tx_buffer[256];  // Adjust size as needed

// MQTT broker details (replace with your broker's details)
#define MQTT_BROKER_HOSTNAME "your_broker_hostname"
#define MQTT_BROKER_PORT     1883  // Use 8883 for TLS

// MQTT client details
#define MQTT_CLIENT_ID       "your_client_id"
#define MQTT_USERNAME        "your_username"  // If applicable
#define MQTT_PASSWORD        "your_password"  // If applicable

// Custom topics as examples
#define TOPIC_SENSOR_DATA    "sensor/data"
#define TOPIC_DEVICE_STATUS  "device/status"
#define TOPIC_CONTROL_CMD    "control/cmd"

static void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                LOG_ERR("MQTT connect failed %d", evt->result);
                break;
            }
            LOG_INF("MQTT connected");
            break;

        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT disconnected");
            break;

        case MQTT_EVT_PUBLISH: {
            const struct mqtt_publish_param *p = &evt->param.publish;
            LOG_INF("MQTT message received: %d bytes on topic %s",
                    p->message.payload.len, p->message.topic.topic.utf8);

            // Handle custom topics
            if (strncmp(p->message.topic.topic.utf8, TOPIC_SENSOR_DATA, p->message.topic.topic.size) == 0) {
                LOG_DBG("Received data on SENSOR_DATA topic");
                // Process sensor data here
            } else if (strncmp(p->message.topic.topic.utf8, TOPIC_DEVICE_STATUS, p->message.topic.topic.size) == 0) {
                LOG_DBG("Received data on DEVICE_STATUS topic");
                // Process device status here
            } else if (strncmp(p->message.topic.topic.utf8, TOPIC_CONTROL_CMD, p->message.topic.topic.size) == 0) {
                LOG_DBG("Received command on CONTROL_CMD topic");
                // Process control command here
            }
            break;
        }

        case MQTT_EVT_PUBACK:
            LOG_INF("PUBACK packet id: %u", evt->param.puback.message_id);
            break;

        case MQTT_EVT_SUBACK:
            LOG_INF("SUBACK packet id: %u", evt->param.suback.message_id);
            break;

        default:
            LOG_WRN("MQTT event of type %d received", evt->type);
            break;
    }
}

int network_client_init(void) {
    // MQTT client configuration
    client.broker.hostname = MQTT_BROKER_HOSTNAME;
    client.broker.port = MQTT_BROKER_PORT;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.password = NULL;  // Set to MQTT_PASSWORD if applicable
    client.username = NULL;  // Set to MQTT_USERNAME if applicable
    client.protocol_version = MQTT_VERSION_3_1_1;

    // MQTT buffers
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    // Connect to the MQTT broker
    int ret = mqtt_connect(&client);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

int mqtt_publish(const char *topic, const char *message) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = message;
    param.message.payload.len = strlen(message);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(&client, &param);
}

int mqtt_subscribe(const char *topic) {
    struct mqtt_subscription_list subscription;

    subscription.list = (struct mqtt_topic *)&topic;
    subscription.list_count = 1;
    subscription.message_id = sys_rand32_get();

    return mqtt_subscribe(&client, &subscription);
}

