#include "dither_neon.h"
#include <arm_neon.h>

#ifndef __ARM_NEON__
#error You should compile this file with '-mfloat-abi=softfp -mfpu=neon'
#endif

/*
 * Dithering algorithm implementation - Y8->Y1 version 2.0 for i.MX
 * Ordered (neon).
 */
void dither_neon_Y1(
	unsigned char *update_region_ptr,
	int width, int height, int stride)
{
	int x, y;
	
	uint8_t threshold1[4] = { 15, 135, 45, 165 };
	uint8_t threshold2[4] = { 195, 75, 225, 105 };
	uint8_t threshold3[4] = { 60, 180, 30, 150 };
	uint8_t threshold4[4] = { 240, 120, 210, 90 };
	
	uint8x16_t q_threshold1 = (uint8x16_t)vld1q_dup_u32((uint32_t const *)threshold1);
	uint8x16_t q_threshold2 = (uint8x16_t)vld1q_dup_u32((uint32_t const *)threshold2);
	uint8x16_t q_threshold3 = (uint8x16_t)vld1q_dup_u32((uint32_t const *)threshold3);
	uint8x16_t q_threshold4 = (uint8x16_t)vld1q_dup_u32((uint32_t const *)threshold4);

	for (y = 0; y < height; y += 4)
	{
		unsigned char *ptr = update_region_ptr + stride*y;
		for (x = 0; x < width; x += 16)
		{
			uint8x16_t l1 = vld1q_u8(ptr);
			l1 = vcgtq_u8(l1, q_threshold1);
			vst1q_u8(ptr, l1);

			uint8x16_t l2 = vld1q_u8(ptr + stride);
			l2 = vcgtq_u8(l2, q_threshold2);
			vst1q_u8(ptr + stride, l2);

			uint8x16_t l3 = vld1q_u8(ptr + stride*2);
			l3 = vcgtq_u8(l3, q_threshold3);
			vst1q_u8(ptr + stride*2, l3);

			uint8x16_t l4 = vld1q_u8(ptr + stride*3);
			l4 = vcgtq_u8(l4, q_threshold4);
			vst1q_u8(ptr + stride*3, l4);

			ptr += 16;
		}
	}
}
