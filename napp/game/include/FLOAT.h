#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "common.h"

#ifdef BIN_SCALE

	static inline int32_t F2int(FLOAT a)  {  return a >> 16;  }
	static inline FLOAT int2F(int32_t a)  {  return a << 16;  }
	static inline FLOAT F_mul_int(FLOAT a, int32_t b)  {  return a * b;  }
	static inline FLOAT F_div_int(FLOAT a, int32_t b)  {  return a / b;  }

	FLOAT f2F(float);
	FLOAT F_mul_F(FLOAT, FLOAT);
	FLOAT F_div_F(FLOAT, FLOAT);
	FLOAT pow(FLOAT, FLOAT);

#else

	static inline int32_t F2int(FLOAT a) {  return a;  }
	static inline FLOAT int2F(int32_t a) {  return a;  }
	static inline FLOAT f2F(float a)     {  return a;  }
	static inline FLOAT F_mul_int(FLOAT a, uint32_t b)  {  return a * b;  }
	static inline FLOAT F_div_int(FLOAT a, uint32_t b)  {  return a / b;  }
	static inline FLOAT F_mul_F(FLOAT a, FLOAT b)  {  return a * b;  }
	static inline FLOAT F_div_F(FLOAT a, FLOAT b)  {  return a / b;  }
	static FLOAT pow(FLOAT x, FLOAT y)  {  assert(0);  }

#endif

#endif
