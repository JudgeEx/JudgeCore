#pragma once
#ifndef __MAKE_OUTPUT_H_
#define __MAKE_OUTPUT_H_
#ifdef __cplusplus
#define EXT_C extern "C"
#else
#include <stdbool.h>
#define EXT_C
#endif
#include "ProcessInfo.h"
EXT_C char *GetOutput(LPPROCESS_RESULT);
#endif
