#pragma once
#include <math.h>
#include "hsvargb.h"

void yellowToBlue(float value, float *R, float *G, float *B) {

	const int color_number = 2;
	static float color[2][3] = { {0,0,1}, {1,1,0} };
	int idx1, idx2;
	float fracBetween = 0;
	if (value < 0) value = 0.001; if (value > 1) value = 0.999;

	idx1 = floor(value);
	idx2 = idx1 + 1;

	float raw = floorf(value * 256.0);
	float fValue = raw / 256.0;

	fracBetween = fValue;

	*R = ((color[idx2][0] - color[idx1][0])*fracBetween + color[idx1][0]);
	*G = ((color[idx2][1] - color[idx1][1])*fracBetween + color[idx1][1]);
	*B = ((color[idx2][2] - color[idx1][2])*fracBetween + color[idx1][2]);
}

void grayscale(float value, float* R, float* G, float* B) {

	*R = value;
	*G = value;
	*B = value;
}

void rainbow(float value, float huebias, float saturationbias, float vbias,float* R, float* G, float* B)
{
	const float dx = 0.8;
	if (value < 0) value = 0; if (value > 1) value = 1;

	float raw = floorf(value * 256.0);
	float fValue = raw / 256.0;

	fValue = (6 - 2 * dx)*fValue + dx;


	float preR = fmax(0.0, (3 - fabs(fValue - 4) - fabs(fValue - 5)) / 2);
	float preG = fmax(0.0, (4 - fabs(fValue - 2) - fabs(fValue - 4)) / 2);
	float preB = fmax(0.0, (3 - fabs(fValue - 1) - fabs(fValue - 2)) / 2);

	float h, s, v;

	rgb2hsv(preR, preG, preB, &h, &s, &v);

	h += huebias;
	s += saturationbias;
	v += vbias;


	float fR, fG, fB;

	hsv2rgb(h, s, v, &fR, &fG, &fB);

	*R = fR;
	*G = fG;
	*B = fB;
}