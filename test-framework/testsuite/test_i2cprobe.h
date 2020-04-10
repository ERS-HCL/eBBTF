#ifndef TEST_I2CPROBE_H
#define TEST_I2CPROBE_H

#include <common.h>
#include "../utils/i2c_utils.h"
#include "../common/process_diagtest_cmds.h"


/**
 * I2C Interface_Test
 *
 * The test probes the I2C devices connected directly to the I2C bus
 */

TestResultTypes_e I2C_interface_test( uint32_t testId, void * arg);

#define MAX_I2C_DEVICE 128


typedef struct {
    int8_t bus_no;
    uint8_t dev_address_list[MAX_I2C_DEVICE];
    const char* dev_name_list[MAX_I2C_DEVICE];
    int8_t no_of_device;
}A_I2C_interface_test;

#endif
