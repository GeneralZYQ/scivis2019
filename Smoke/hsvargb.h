#pragma once
#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

//void rgb2hsv(float r, float g, float b, float *h, float *s, float *v);

void rgb2hsv(float r, float g, float b, float *h, float *s, float *v) {
	float M = fmax(r, fmax(g, b));
	float m = fmin(r, fmin(g, b));

	float d = M - m;

	*v = M;
	*s = (M > 0.000001) ? d / M : 0;

	if (s == 0) {
		h = 0;
	}
	else {
		if (r == M) {
			*h = (g - b) / d;
		}
		else if (g == M)
		{
			*h = 2 + (b - r) / d;
		}
		else
		{
			*h = 4 + (r - g) / d;
		}
		*h /= 6;
		if (*h < 0) {
			*h += 1;
		}
	}
}

void hsv2rgb(float h, float s, float v, float *r, float *g, float *b) {

	int hueCase = (int)(h * 6);
	float frac = 6 * h - hueCase;
	float lx = v * (1 - s);
	float ly = v * (1 - s * frac);
	float lz = v * (1 - s * (1 - frac));
	switch (hueCase)
	{
	case 0:
	case 6: *r = v; *g = lz; *b = lx; break; // 0<hue<1/6
	case 1: *r = ly; *g = v; *b = lx; break; // 1/6<hue<2/6
	case 2: *r = lx; *g = v; *b = lz; break; // 2/6<hue<3/6
	case 3: *r = lx; *g = ly; *b = v; break; // 3/6<hue/4/6
	case 4: *r = lz; *g = lx; *b = v; break; // 4/6<hue<5/6
	case 5: *r = v; *g = lx; *b = ly; break; // 5/6<hue<1
	}
}

#endif