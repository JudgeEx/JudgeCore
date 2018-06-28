#pragma once
#ifndef __CONFIG_LOADER_H_
#define __CONFIG_LOADER_H_
#ifdef __cplusplus
#define EXT_C extern "C"
#else
#include <stdbool.h>
#define EXT_C
#endif
EXT_C struct
{
    bool Redirect;
    char *CommandLine;
    unsigned long TimeLimit;
    unsigned long MemoryLimit;
} config;
EXT_C bool LoadConfig(const char *);
#endif
