#ifndef SINCOS_H_
#define SINCOS_H_

#include <inttypes.h>

/*#define M_PI_X16	(int32_t)(M_PI * 65536.0) */
#define M_PI_X16	(int32_t)((31416 << 16) / 10000)

#define SINLUT_SCALE	512
#define SINLUT_SIZE		512
int16_t sinlut[SINLUT_SIZE];

void sincos_init(void);

/* takes angle in [0, SINLUT_SIZE] and returns:
 * sin(2 * angle / SINLUT_SIZE / pi) * SINLUT_SCALE
 */
int16_t sin_int(int16_t norm_angle);
int16_t cos_int(int16_t norm_angle);

/* takes angle in fixed point 16.16 radians [0, 2pi << 16]
 * and returns 16.16 fixed point in [-1 << 16, 1 << 16]
 */
int32_t sin_x16(int32_t radians);
int32_t cos_x16(int32_t radians);

#endif	/* SINCOS_H_ */
