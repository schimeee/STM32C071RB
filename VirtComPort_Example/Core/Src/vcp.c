#include "vcp.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include "stm32c0xx_nucleo.h"   /* for COM types + hcom_uart[] */

/* BSP COM init struct */
static COM_InitTypeDef s_comInit;

/* Provided by BSP after BSP_COM_Init() */
extern UART_HandleTypeDef hcom_uart[];

/* ST-LINK VCP is usually COM1 (index 0) */
#ifndef VCP_COM_PORT
#define VCP_COM_PORT COM1
#endif

/* printf buffer size */
#ifndef VCP_PRINTF_BUFSZ
#define VCP_PRINTF_BUFSZ 256
#endif

int VCP_Init(uint32_t baud)
{
  s_comInit.BaudRate   = baud;
  s_comInit.WordLength = COM_WORDLENGTH_8B;
  s_comInit.StopBits   = COM_STOPBITS_1;
  s_comInit.Parity     = COM_PARITY_NONE;
  s_comInit.HwFlowCtl  = COM_HWCONTROL_NONE;

  if (BSP_COM_Init(VCP_COM_PORT, &s_comInit) != BSP_ERROR_NONE)
  {
    return -1;
  }
  return 0;
}

int VCP_Write(const void *data, uint16_t len)
{
  if (!data || !len) return 0;

  if (HAL_UART_Transmit(&hcom_uart[VCP_COM_PORT],
                        (uint8_t*)data, len, HAL_MAX_DELAY) == HAL_OK)
    return (int)len;

  return -1;
}

int VCP_Printf(const char *fmt, ...)
{
  if (!fmt) return -1;

  char buf[VCP_PRINTF_BUFSZ];
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  if (n < 0) return -1;

  uint16_t to_send = (n < (int)sizeof(buf)) ? (uint16_t)n : (uint16_t)(sizeof(buf) - 1);
  return VCP_Write(buf, to_send);
}
