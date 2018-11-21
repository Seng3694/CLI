#ifndef SENG_COMMON_H
#define SENG_COMMON_H

#ifdef __cplusplus
#define SENGAPIBEGIN extern "C" {
#define SENGAPIEND }
#else
#define SENGAPIBEGIN 
#define SENGAPIEND 
#endif

SENGAPIBEGIN

#define CLI_FALSE (boolean)0
#define CLI_TRUE (boolean)1

typedef unsigned char boolean;
typedef signed int	  int32;
typedef unsigned int  uint32;

/**
* \brief Macro for checking whether a flag is set or not.
* example: typedef enum { RED = 1, GREEN = 2, BLUE = 4 } Colors;
* binary representation (RED=0001 GREEN=0010 BLUE=0100)
* Colors c = RED | BLUE;  => (0001 | 0100 = 0101)
* HAS_FLAG(c, RED)   => true  => 0101 & 0001 = 0001 == 0001
* HAS_FLAG(c, GREEN) => false => 0101 & 0010 = 0000 != 0010
* HAS_FLAG(c, BLUE)  => true  => 0101 & 0100 = 0100 == 0100
*/
#define HAS_FLAG(value, flag) ((value & flag) == flag)

SENGAPIEND

#endif 
