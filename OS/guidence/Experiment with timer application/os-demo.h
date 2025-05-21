#ifndef __OS_DEMO_H_
#define __OS_DEMO_H_

#include <msgQLib.h>
#include <taskLib.h>
#include <eventLib.h>

extern int os_demo_ip_cache_create_counter;
extern int os_demo_ip_cache_delete_counter;

extern unsigned long Print(char *format, ...);

#endif
