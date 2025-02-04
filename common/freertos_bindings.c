// Copyright (C) 2016-2018 Free Software Foundation, Inc.
//
// GNAT is free software;  you can  redistribute it  and/or modify it under
// terms of the  GNU General Public License as published  by the Free Soft-
// ware  Foundation;  either version 3,  or (at your option) any later ver-
// sion.  GNAT is distributed in the hope that it will be useful, but WITH-
// OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.
//
// As a special exception under Section 7 of GPL version 3, you are granted
// additional permissions described in the GCC Runtime Library Exception,
// version 3.1, as published by the Free Software Foundation.
//
// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.
//
// This interface to FreeRTOS is part of the Cortex GNAT RTS project.
//
// This file creates callable interfaces to some FreeRTOS macros.  The
// alternative is to code in Ada what the macros translate to; this is
// all very well until FreeRTOS changes the way the macro is
// implemented, and in any case the main reason for there being macros
// in the first place is that the implementation is different for
// different CPU architectures.

#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include "soc/spinlock.h"

const int _gnat_freertos_tick_rate = configTICK_RATE_HZ;

void _gnat_disable_interrupts(void) {
  taskDISABLE_INTERRUPTS();
}

void _gnat_enable_interrupts(void) {
  taskENABLE_INTERRUPTS();
}

portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

void _gnat_enter_critical(void) {
  taskENTER_CRITICAL(&myMutex);
}

void _gnat_exit_critical(void) {
  taskEXIT_CRITICAL(&myMutex);
}

void _gnat_yield_from_isr(int switch_required) {
  // portEND_SWITCHING_ISR(switch_required);
  portYIELD_FROM_ISR(switch_required);
}

xQueueHandle _gnat_xQueueCreate(unsigned portBASE_TYPE uxQueueLength,
                                unsigned portBASE_TYPE uxItemSize) {
  return xQueueCreate(uxQueueLength, uxItemSize);
}

// We only need to append new items to the queue.
portBASE_TYPE _gnat_xQueueSend(xQueueHandle xQueue,
                               const void *pvItemToQueue,
                               portTickType xTicksToWait) {
  return xQueueSendToBack(xQueue, pvItemToQueue, xTicksToWait);
}

portBASE_TYPE _gnat_xQueueSendFromISR
  (xQueueHandle xQueue,
   const void *pvItemToQueue,
   portBASE_TYPE *pxHigherPriorityTaskWoken) {
  return xQueueSendToBackFromISR
    (xQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

portBASE_TYPE _gnat_xQueueOverwrite(xQueueHandle xQueue,
                                    const void *pvItemToQueue) {
  return xQueueOverwrite(xQueue, pvItemToQueue);
}

portBASE_TYPE _gnat_xQueueReceive(xQueueHandle xQueue,
                                  void *pvBuffer,
                                  portTickType xTicksToWait) {
  return xQueueReceive(xQueue, pvBuffer, xTicksToWait);
}

xSemaphoreHandle _gnat_xSemaphoreCreateBinary() {
  return xSemaphoreCreateBinary();
}

portBASE_TYPE _gnat_xSemaphoreGive(xSemaphoreHandle xSemaphore) {
  return xSemaphoreGive(xSemaphore);
}

portBASE_TYPE _gnat_xSemaphoreTake(xSemaphoreHandle xSemaphore,
                                   portTickType xBlockTime) {
  return xSemaphoreTake(xSemaphore, xBlockTime);
}

portBASE_TYPE _gnat_xSemaphoreGiveFromISR
  (xSemaphoreHandle xSemaphore,
   portBASE_TYPE *pxHigherPriorityTaskWoken) {
  return xSemaphoreGiveFromISR(xSemaphore, pxHigherPriorityTaskWoken);
}


/* xTaskCreate has `static inline` markers in IDF, so can't be linked */
IRAM_ATTR BaseType_t xTaskCreate2
  (TaskFunction_t pvTaskCode,
   const char * const pcName,
   const uint16_t usStackDepth,
   void * const pvParameters,
   UBaseType_t uxPriority,
   TaskHandle_t * const pvCreatedTask) {
  return xTaskCreate( pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pvCreatedTask );
}

