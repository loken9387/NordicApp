#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <zephyr.h>

typedef enum {
    PAYLOAD_TYPE_SENSOR_DATA,
    PAYLOAD_TYPE_DEVICE_STATUS,
    PAYLOAD_TYPE_CONTROL_CMD,
    PAYLOAD_TYPE_TEMP_DATA,
    // ... add other payload types as needed ...
} PayloadType;

typedef struct {
    double temperature;
    char device_id[32];  // Assuming a max device ID length of 31 characters + null terminator
} TempDataPayload;

/* Convert sensor data to JSON string */
int sensor_data_to_json(const struct sensor_value *data, char *json_buffer, size_t buffer_size);

/* Parse JSON string to extract specific data (e.g., server response) */
int json_parse_response(const char *json_str, char *parsed_data, size_t data_size);

#endif // JSON_HANDLER_H

