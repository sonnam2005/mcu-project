#ifndef HAL_COMMON_H
#define HAL_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include "FreeRTOS.h"

typedef enum { HAL_OK = 0, HAL_ERR, HAL_TIMEOUT, HAL_BUSY } hal_status_t;

#endif
