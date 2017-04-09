#ifndef FIXED_H_
#define FIXED_H_

#include <inttypes.h>

#define ftox16(x)		(int32_t)((x) * 65536.0f)
#define itox16(x)		(int32_t)((x) << 16)

#define x16tof(x)		(float)((x) / 65536.0f)
#define x16toi(x)		((x) >> 16)

#define x16mul(a, b)	(int32_t)(((int32_t)(a) >> 8) * ((int32_t)(b) >> 8))
/*int32_t x16div(register int32_t num, register int32_t denom);*/
#define x16div(a, b)	(int32_t)(((int64_t)(a) << 16) / (b))

#define x16sq(x)		x16mul(x, x)

#endif	/* FIXED_H_ */
