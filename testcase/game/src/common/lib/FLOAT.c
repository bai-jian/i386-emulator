#include "common.h"

#ifdef BIN_SCALE

#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b)  {  return a * b >> 16;  }
FLOAT F_div_F(FLOAT a, FLOAT b)  {  return a / b << 16;  }

FLOAT f2F(float a)
{
	volatile void* p = &a;
	volatile uint32_t b = *(uint32_t*)p;
	uint32_t sign_b = b >> 31;
	uint32_t expo_b = ((b >> 23) & 0x000000FF) - 127;
	uint32_t mati_b = (b & 0x007FFFFF) + (1 << 23);
	uint32_t valu_b = mati_b >> (23 - 16 - expo_b);
	return  (sign_b << 31) + valu_b;
}

static inline FLOAT Fabs(FLOAT x) {
	assert(0);
	return 0;
}


FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^(1/3) */
	FLOAT t, t2, dt, next_t = int2F(2);

	do {
		t = next_t;
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		next_t = t + dt;
	} while(Fabs(dt) > f2F(1e-3));

	return next_t;
}

#endif
