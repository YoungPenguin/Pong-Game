// =====================================================================
//
//	Exported by Cearn's excellut v1.0
//	(comments, kudos, flames to daytshen@hotmail.com)
//
// =====================================================================
#include "stm32f30x_conf.h"
#include "30010_io.h"

#ifndef MATH_H
#define MATH_H
#define FIX14_SHIFT 14
#define FIX14_MULT(a,b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a,b) ( ((a) << FIX14_SHIFT) / (b) )

// === LUT SIZES ===
#define SIN_SIZE 512
typedef struct Vector{
  int32_t x, y;
} vector_t;

// === LUT DECLARATIONS ===
extern const signed short SIN[512];
int32_t sin(int32_t point);
int32_t cos(int32_t point);
void initVector(vector_t *v, int32_t x0, int32_t y0);
void rotate(vector_t *v, int32_t angle);
void printFix(int32_t i);
int32_t expand(int32_t i);

#endif	// MATH_H
