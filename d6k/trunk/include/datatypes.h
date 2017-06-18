
#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H

//基本数据类型

//Unsigned  8 bit quantity
#ifndef INT8U
typedef unsigned char INT8U;
#endif

#ifndef int8u
typedef unsigned char int8u;
#endif

//Signed 8 bit quantity
#ifndef INT8S
typedef char  INT8S;
#endif

#ifndef int8s
typedef char  int8s;                    
#endif

//Unsigned 16 bit quantity
#ifndef INT16U
typedef unsigned short INT16U;
#endif

#ifndef int16u
typedef unsigned short int16u;
#endif

//Signed   16 bit quantity
#ifndef INT16S
typedef short   INT16S;
#endif

#ifndef int16s
typedef short   int16s;
#endif


//Unsigned 32 bit quantity
#ifndef INT32U
typedef unsigned long  INT32U;
#endif

#ifndef int32u
typedef unsigned long  int32u;
#endif

#ifndef INT64U
typedef unsigned long long  INT64U;
#endif

#ifndef int64u
typedef unsigned long long  int64u;
#endif

//Signed   32 bit quantity
#ifndef INT32S
typedef long  INT32S;
#endif

#ifndef INT64S
typedef long long INT64S;
#endif

#ifndef int32s
typedef long  int32s;
#endif

#ifndef int64s
typedef long long int64s;
#endif

//Single precision floating point
#ifndef FP32
typedef float          FP32;
#endif

#ifndef fp32
typedef float          fp32;
#endif

//Double precision floating point
#ifndef FP64
typedef double         FP64;
#endif

#ifndef fp64
typedef double         fp64;
#endif


enum KRESULT
{
	K_TRUE,
	K_FALSE,
};


#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if( (x) != NULL ) { delete (x); (x) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if( (x) != NULL ) { delete[] (x); (x) = NULL; }
#endif


#endif // _DATA_TYPES_H


