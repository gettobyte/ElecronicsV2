#include "BNO055.h"
#include "lpi2c_driver.h"
#include "osif.h"   /* For OSIF_TimeDelay */

/* ========= Configuration ========= */

/* LPI2C instance index from your SDK config (usually 0 for LPI2C0) */
#define BNO055_LPI2C_INSTANCE       (0u)

/* If you want the address here just for reference */
#define BNO055_I2C_ADDR_7BIT        (0x28u)    /* or 0x29u depending on your board */

/* BNO055 registers (minimal set) */
#define BNO055_CHIP_ID_ADDR         (0x00u)
#define BNO055_PAGE_ID_ADDR         (0x07u)
#define BNO055_EUL_HEADING_LSB      (0x1Au)    /* 6 bytes: H_L, H_M, R_L, R_M, P_L, P_M */
#define BNO055_UNIT_SEL_ADDR        (0x3Bu)
#define BNO055_OPR_MODE_ADDR        (0x3Du)
#define BNO055_PWR_MODE_ADDR        (0x3Eu)
#define BNO055_SYS_TRIGGER_ADDR     (0x3Fu)

/* Expected chip ID */
#define BNO055_CHIP_ID_VALUE        (0xA0u)

/* Operation modes */
#define BNO055_OPMODE_CONFIG        (0x00u)
#define BNO055_OPMODE_NDOF          (0x0Cu)

/* Power modes */
#define BNO055_PWRMODE_NORMAL       (0x00u)

/* Page IDs */
#define BNO055_PAGE_ZERO            (0x00u)

/* Units:
 *  0x00 = accel m/s^2, gyro dps, euler degrees, temp Celsius, Windows format
 */
#define BNO055_UNIT_SEL_DEFAULT     (0x00u)

/* Timing */
#define BNO055_MODE_CHANGE_DELAY_MS (25u)
#define BNO055_RESET_DELAY_MS       (650u)
#define BNO055_I2C_TIMEOUT_MS       (100u)

/* ========= Low-level I2C helpers ========= */

/* Raw write: just send 'tx' bytes on the configured instance */
static int bno055_i2c_write_raw(const uint8_t *tx, uint8_t len)
{
    if ((tx == NULL) || (len == 0u)) {
        return -1;
    }

    status_t status = LPI2C_DRV_MasterSendDataBlocking(
        BNO055_LPI2C_INSTANCE,
        tx,
        (uint32_t)len,
        true,  // sendStop flag
        BNO055_I2C_TIMEOUT_MS
    );

    return (status == STATUS_SUCCESS) ? 0 : -1;
}

/* Read 'len' bytes from register 'reg' */
static int bno055_i2c_read_reg(uint8_t reg, uint8_t *rx, uint8_t len)
{
    if ((rx == NULL) || (len == 0u)) {
        return -1;
    }

    status_t status;

    /* 1) Send the register address */
    status = LPI2C_DRV_MasterSendDataBlocking(
        BNO055_LPI2C_INSTANCE,
        &reg,
        1u,
        true,  // sendStop flag
        BNO055_I2C_TIMEOUT_MS
    );
    if (status != STATUS_SUCCESS) {
        return -1;
    }

    /* 2) Read data bytes */
    status = LPI2C_DRV_MasterReceiveDataBlocking(
        BNO055_LPI2C_INSTANCE,
        rx,
        (uint32_t)len,
        true,  // sendStop flag
        BNO055_I2C_TIMEOUT_MS
    );
    if (status != STATUS_SUCCESS) {
        return -1;
    }

    return 0;
}

/* ========= Register-level helpers ========= */

static bno055_status_t bno055_write8(uint8_t reg, uint8_t val)
{
    uint8_t tx[2];
    tx[0] = reg;
    tx[1] = val;

    if (bno055_i2c_write_raw(tx, 2u) != 0) {
        return BNO055_STATUS_I2C_ERROR;
    }
    return BNO055_STATUS_OK;
}

static bno055_status_t bno055_read8(uint8_t reg, uint8_t *val)
{
    if (val == NULL) {
        return BNO055_STATUS_INVALID_PARAM;
    }

    if (bno055_i2c_read_reg(reg, val, 1u) != 0) {
        return BNO055_STATUS_I2C_ERROR;
    }

    return BNO055_STATUS_OK;
}

static bno055_status_t bno055_read_len(uint8_t start_reg, uint8_t *buf, uint8_t len)
{
    if ((buf == NULL) || (len == 0u)) {
        return BNO055_STATUS_INVALID_PARAM;
    }

    if (bno055_i2c_read_reg(start_reg, buf, len) != 0) {
        return BNO055_STATUS_I2C_ERROR;
    }

    return BNO055_STATUS_OK;
}

/* ========= Public API ========= */

bno055_status_t bno055_init(void)
{
    bno055_status_t st;
    uint8_t id = 0;

    /* 1. Read chip ID */
    st = bno055_read8(BNO055_CHIP_ID_ADDR, &id);
    if (st != BNO055_STATUS_OK) {
        return st;
    }
    if (id != BNO055_CHIP_ID_VALUE) {
        return BNO055_STATUS_BAD_CHIP_ID;
    }

    /* 2. Enter CONFIG mode */
    st = bno055_write8(BNO055_OPR_MODE_ADDR, BNO055_OPMODE_CONFIG);
    if (st != BNO055_STATUS_OK) {
        return st;
    }
    OSIF_TimeDelay(BNO055_MODE_CHANGE_DELAY_MS);

    /* 3. Optional: soft reset */
    st = bno055_write8(BNO055_SYS_TRIGGER_ADDR, 0x20u); /* RST_SYS */
    if (st != BNO055_STATUS_OK) {
        return st;
    }
    OSIF_TimeDelay(BNO055_RESET_DELAY_MS);

    /* 4. Re-read chip ID after reset */
    st = bno055_read8(BNO055_CHIP_ID_ADDR, &id);
    if (st != BNO055_STATUS_OK) {
        return st;
    }
    if (id != BNO055_CHIP_ID_VALUE) {
        return BNO055_STATUS_BAD_CHIP_ID;
    }

    /* 5. Ensure Page 0 */
    st = bno055_write8(BNO055_PAGE_ID_ADDR, BNO055_PAGE_ZERO);
    if (st != BNO055_STATUS_OK) {
        return st;
    }

    /* 6. Power mode NORMAL */
    st = bno055_write8(BNO055_PWR_MODE_ADDR, BNO055_PWRMODE_NORMAL);
    if (st != BNO055_STATUS_OK) {
        return st;
    }

    /* 7. Unit selection (deg + °C) */
    st = bno055_write8(BNO055_UNIT_SEL_ADDR, BNO055_UNIT_SEL_DEFAULT);
    if (st != BNO055_STATUS_OK) {
        return st;
    }

    /* 8. Clear SYS_TRIGGER (no self-test, ext clock off) */
    st = bno055_write8(BNO055_SYS_TRIGGER_ADDR, 0x00u);
    if (st != BNO055_STATUS_OK) {
        return st;
    }

    /* 9. Switch to NDOF */
    st = bno055_write8(BNO055_OPR_MODE_ADDR, BNO055_OPMODE_NDOF);
    if (st != BNO055_STATUS_OK) {
        return st;
    }
    OSIF_TimeDelay(BNO055_MODE_CHANGE_DELAY_MS);

    return BNO055_STATUS_OK;
}

bno055_status_t bno055_read_euler_deg(bno055_euler_t *out)
{
    if (out == NULL) {
        return BNO055_STATUS_INVALID_PARAM;
    }

    uint8_t buf[6];
    bno055_status_t st;

    st = bno055_read_len(BNO055_EUL_HEADING_LSB, buf, sizeof(buf));
    if (st != BNO055_STATUS_OK) {
        return st;
    }

    int16_t raw_h = (int16_t)((buf[1] << 8) | buf[0]);  /* heading */
    int16_t raw_r = (int16_t)((buf[3] << 8) | buf[2]);  /* roll */
    int16_t raw_p = (int16_t)((buf[5] << 8) | buf[4]);  /* pitch */

    const float scale = 1.0f / 16.0f;  /* 1 LSB = 1/16 degree */

    out->heading_deg = (float)raw_h * scale;
    out->roll_deg    = (float)raw_r * scale;
    out->pitch_deg   = (float)raw_p * scale;

    return BNO055_STATUS_OK;
}
