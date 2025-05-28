
#ifndef __CONFIG_H
#define __CONFIG_H
/*****��غ궨��***/
/* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       long int int64_t;

/* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       long int uint64_t;

#define ENABLE  1
#define DISABLE  0


/****ͷ�ļ�����*****/
#include <intrins.h>
#include "STC11XX.H"
#include "LDChip.h"
#include "Reg_RW.h"
#include "usart.h"

/****�������붨��*******/
#define TEST		 //��������



#endif
