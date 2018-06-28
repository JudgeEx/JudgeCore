#pragma once
#ifndef __PROCESSINFO_H_
#define __PROCESSINFO_H_
#ifdef __cplusplus
#define EXT_C extern "C"
#else
#include <stdbool.h>
#define EXT_C
#endif
EXT_C typedef struct PROCESS_RESULT
{
    unsigned ReturnCode;
    unsigned long long ExecutionTime;
    unsigned long long PeakWorkingSetSize;
} * LPPROCESS_RESULT;
EXT_C typedef struct PROCESS_LIMIT
{
    unsigned ExecutionTime;
} * LPPROCESS_LIMIT;
#endif
