
#include <math.h>

/*


void rgb2hsv(float r, float g, float b, float *h, float *s, float *v) {
	float M = fmax(r, fmax(g, b));
	float m = fmin(r, fmin(g, b));

	float d = M - m;

	*v = M;
	*s = (M > 0.000001) ? d/M : 0;

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
*/