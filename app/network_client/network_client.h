#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <zephyr.h>

/* Initialize the network client (MQTT) */
int network_client_init(void);

/* Publish a message to a specific MQTT topic */
int mqtt_publish(const char *topic, const char *message);

/* Subscribe to a specific MQTT topic */
int mqtt_subscribe(const char *topic);

#endif // NETWORK_CLIENT_H

