#ifndef VCP_H_
#define VCP_H_

#include <stdint.h>
#include <stdarg.h>
#include "main.h"   /* pulls HAL + BSP types */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the Virtual COM Port (ST-LINK VCP via BSP COM1).
 * @param baud Baud rate, e.g., 115200
 * @return 0 on success, -1 on failure
 */
int VCP_Init(uint32_t baud);

/**
 * Send raw bytes over VCP (blocking).
 * @param data Pointer to bytes
 * @param len  Number of bytes
 * @return number of bytes sent, or <0 on error
 */
int VCP_Write(const void *data, uint16_t len);

/**
 * printf-like helper that formats then sends (not ISR-safe).
 * @return number of characters sent, or <0 on error
 */
int VCP_Printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* VCP_H_ */
