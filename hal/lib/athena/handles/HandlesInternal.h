/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <stdint.h>

#include "HAL/Handles.h"

/* General Handle Data Layout
 * Bits 0-15:  Handle Index
 * Bits 16-23: Unused
 * Bits 24-30: Handle Type
 * Bit 31:     1 if handle error, 0 if no error
 *
 * Other specialized handles will use different formats, however Bits 24-31 are
 * always reserved for type and error handling.
 */

namespace hal {

enum class HalHandleEnum { Undefined = 0, DIO = 1, Port = 2, Notifier = 3 };

static inline int16_t getHandleIndex(HalHandle handle) {
  // mask and return last 16 bits
  return (int16_t)(handle & 0xffff);
}
static inline HalHandleEnum getHandleType(HalHandle handle) {
  // mask first 8 bits and cast to enum
  return (HalHandleEnum)((handle >> 24) & 0xff);
}
static inline bool isHandleType(HalHandle handle, HalHandleEnum handleType) {
  return handleType == getHandleType(handle);
}
static inline int16_t getHandleTypedIndex(HalHandle handle,
                                          HalHandleEnum enumType) {
  if (!isHandleType(handle, enumType)) return HAL_HANDLE_INVALID_TYPE;
  return getHandleIndex(handle);
}

HalHandle createHandle(int16_t index, HalHandleEnum handleType);
}
