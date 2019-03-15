#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

void getCorrectCoordinates(float x1, float y1, float height, float width, float vecx, float vecy,float *c1x, float *c1y, float *c2x, float *c2y, float *c3x, float *c3y) {

	//in this function , we can get the final coordinations of the three vertices of a triangel for divergence.
	// Firstly we need a little axises for the cell

	if (vecx > 0 && vecy > 0)
	{
		float tana = vecy / vecx;
		float x = (height / 2.0) / tana;
		float y = height / 2.0;

		float knownAngle = atan(tana);
		float innerAngle = M_PI - knownAngle - (15.0 / 180.0) * M_PI;
		float gisAngle = M_PI - innerAngle;
		float gisTan = tan(gisAngle);

		float b = y - gisTan * x;
		float finnalX = (-height / 2.0 - b) / gisTan; // the first pair of coordinations;
		float finnalY = -height / 2.0;


		float secondAngle = M_PI - (M_PI - knownAngle) - (15.0 / 180.0) * M_PI;
		float secondB = y - tan(secondAngle) * (height / 2.0);
		float secondX = -width / 2.0;
		float secondY = tan(secondAngle) * secondX + secondB;

		*c1x = vecx + width / 2.0 + x;
		*c1y = vecy;

		*c2x = vecx + width / 2.0 + finnalX;
		*c2y = vecy + height;

		*c3x = vecx;
		*c3y = vecy + height / 2.0 + secondY;
	}
	else if (vecx < 0 && vecy > 0)
	{
		float tana = vecy / vecx;
		float firstX = (height / 2.0) / tana;
		float y = height / 2.0;

		float innerAngle = M_PI - atan(tana);
		float firstAngle = M_PI - innerAngle - (15.0 / 180.0) * M_PI;
		float firstA = tan(firstAngle);
		float firstB = height / 2.0 - firstA * firstX;
		float firstFinalX = ((-height / 2.0) - firstB) / firstA;
		float firstFinalY = -height / 2.0;

		float secondInnerAngle = atan(tana);
		float secondAngle = M_PI - (M_PI - secondInnerAngle - (15.0 / 180.0) * M_PI);
		float secondB = height / 2.0 - tan(secondAngle) * firstX;
		float secondFinalY = tan(secondAngle) * (width / 2.0) + secondB;

	}
	else if (vecx > 0 && vecy < 0) {
		float tana = vecy / vecx;
		float firstX = (-height / 2.0) / tana;

		float shouldAngle = M_PI - (M_PI - atan(tana)) - (15.0 / 180.0) * M_PI;
		float firstA = tan(shouldAngle);
		float firstB = (-height / 2.0) - firstA * firstX;
		float firstFinalY = (-height / 2.0);
		float firstFinalX = firstX;

		float secondY = height / 2.0;
		float secondX = (secondY - firstB) / firstA;

		float secondAngle = atan(tana) + (15.0 / 180.0) * M_PI;
		float secondA = tan(secondAngle);
		float secondB = (-height / 2.0) - secondA * firstX;
		float thirdFinalY = secondA * (-width / 2.0) + secondB;
		float thirdFinalX = (-width / 2.0);
	}
	else if (vecx < 0 && vecy < 0)
	{
		float tana = vecy / vecx;
		float firstX = (-height / 2.0) / tana;
		float firstY = -height / 2.0;

		float firstAngle = atan(tana) + (15.0 / 180.0) * M_PI;
		float firstA = tan(firstAngle);
		float firstB = (-height / 2.0) - firstA * firstX;
		float secondY = height / 2.0;
		float secondX = (secondY - firstB) / firstA;

		float secondAngle = atan(tana) - (15.0 / 180.0) * M_PI;
		float secondB = firstY - firstX * tan(secondAngle);
		float thirdX = width / 2.0;
		float thirdY = thirdX * tan(secondAngle) + secondB;
	}
}