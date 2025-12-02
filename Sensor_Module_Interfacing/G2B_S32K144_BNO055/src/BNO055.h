/* BNO055.h – minimal S32K driver interface */

#ifndef BNO055_H
#define BNO055_H

#include <stdint.h>

typedef enum {
    BNO055_STATUS_OK = 0,
    BNO055_STATUS_I2C_ERROR,
    BNO055_STATUS_BAD_CHIP_ID,
    BNO055_STATUS_INVALID_PARAM,
} bno055_status_t;

/* Euler angles in degrees */
typedef struct {
    float heading_deg;  /* yaw */
    float roll_deg;
    float pitch_deg;
} bno055_euler_t;

/**
 * Initialize BNO055 on the configured LPI2C instance.
 * - Checks chip ID
 * - Resets sensor
 * - Sets power mode NORMAL
 * - Sets units (degrees + Celsius)
 * - Switches to NDOF fusion mode
 */
bno055_status_t bno055_init(void);

/**
 * Read Euler angles (Heading, Roll, Pitch) in degrees.
 */
bno055_status_t bno055_read_euler_deg(bno055_euler_t *out);

#endif /* BNO055_H */
