#pragma once
#define _USE_MATH_DEFINES
#include <math.h>


// x1: start x, y1 : start y( start x and start y is the left bottom corner of the cell), height: cell height, 
//width : cell widht , vecx : xVelocity, vecy : y Velocity 

/// returns
// c1x, c1y : the first pair of the traingle vertex coordination
// c2x , c2y: the second pair of the traingle vertex coordination
// c3x , c3y: the second pair of the traingle vertex coordination
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
		if (gisAngle == M_PI / 2.0)
		{
			gisAngle = gisAngle + 0.01;
		}
		float gisTan = tan(gisAngle);

		float b = y - gisTan * x;
		float finnalX = (-height / 2.0 - b) / gisTan; // the first pair of coordinations;
		float finnalY = -height / 2.0;


		float secondAngle = M_PI - (M_PI - knownAngle) - (15.0 / 180.0) * M_PI;
		if (secondAngle == M_PI / 2.0)
		{
			secondAngle = secondAngle + 0.01;
		}
		float secondB = y - tan(secondAngle) * (height / 2.0);
		float secondX = -width / 2.0;
		float secondY = tan(secondAngle) * secondX + secondB;

		*c1x = max(min(x1 + width / 2.0 + x, x1 + width), x1)  ;
		*c1y = y1 + height;

		*c2x = max(x1, min(x1 + width / 2.0 + finnalX, x1 + width)) ;
		*c2y = y1;

		*c3x = x1;
		*c3y = max(min(y1 + height / 2.0 + secondY, y + height), y1);
	}
	else if (vecx < 0 && vecy > 0)
	{
		float tana = vecy / vecx;
		float firstX = (height / 2.0) / tana;
		float y = height / 2.0;

		float innerAngle = M_PI - atan(tana);
		float firstAngle = M_PI - innerAngle - (15.0 / 180.0) * M_PI;
		if (firstAngle == M_PI / 2.0)
		{
			firstAngle = firstAngle + 0.01;
		}
		float firstA = tan(firstAngle);
		float firstB = height / 2.0 - firstA * firstX;
		float firstFinalX = ((-height / 2.0) - firstB) / firstA;
		float firstFinalY = -height / 2.0;

		float secondInnerAngle = atan(tana);
		float secondAngle = M_PI - (M_PI - secondInnerAngle - (15.0 / 180.0) * M_PI);
		if (secondAngle == M_PI / 2.0)
		{
			secondAngle = secondAngle + 0.01;
		}
		float secondB = height / 2.0 - tan(secondAngle) * firstX;
		float secondFinalY = tan(secondAngle) * (width / 2.0) + secondB;

		*c1x = max(x1, min(x1 + width / 2.0 + firstX, x1 + width))  ;
		*c1y = y1 + height;

		*c2x = max(x1, min(x1 + width / 2.0 + firstFinalX, x1 + width))  ;
		*c2y = y1;

		*c3x = x1 + width;
		*c3y = max(y1, min(y1 + height / 2.0 + secondFinalY, y1 + height)) ;

	}
	else if (vecx > 0 && vecy < 0) {
		float tana = vecy / vecx;
		float firstX = (-height / 2.0) / tana;

		float shouldAngle = M_PI - (M_PI - atan(tana)) - (15.0 / 180.0) * M_PI;
		if (shouldAngle == M_PI / 2.0)
		{
			shouldAngle = shouldAngle + 0.01;
		}
		float firstA = tan(shouldAngle);
		float firstB = (-height / 2.0) - firstA * firstX;
		float firstFinalY = (-height / 2.0);
		float firstFinalX = firstX;

		float secondY = height / 2.0;
		float secondX = (secondY - firstB) / firstA;

		float secondAngle = atan(tana) + (15.0 / 180.0) * M_PI;
		if (secondAngle == M_PI / 2.0)
		{
			secondAngle = secondAngle + 0.001;
		}
		float secondA = tan(secondAngle);
		float secondB = (-height / 2.0) - secondA * firstX;
		float thirdFinalY = secondA * (-width / 2.0) + secondB;
		float thirdFinalX = (-width / 2.0);

		*c1x = max(x1, min(x1 + width / 2.0 + firstX, x1 + width)) ;
		*c1y = y1;

		*c2x = max(x1, min(x1 + width / 2.0 + secondX, x1 + width)) ;
		*c2y = y1 + height;

		*c3x = x1;
		*c3y = max(y1, min(y1 + height / 2.0 + thirdFinalY, y1 + height)) ;
	}
	else if (vecx < 0 && vecy < 0)
	{
		float tana = vecy / vecx;
		float firstX = (-height / 2.0) / tana;
		float firstY = -height / 2.0;

		float firstAngle = atan(tana) + (15.0 / 180.0) * M_PI;
		if (firstAngle == M_PI / 2.0)
		{
			firstAngle = firstAngle + 0.001;
		}
		float firstA = tan(firstAngle);
		float firstB = (-height / 2.0) - firstA * firstX;
		float secondY = height / 2.0;
		float secondX = (secondY - firstB) / firstA;

		float secondAngle = atan(tana) - (15.0 / 180.0) * M_PI;
		if (secondAngle == M_PI / 2.0)
		{
			secondAngle = secondAngle + 0.001;
		}
		float secondB = firstY - firstX * tan(secondAngle);
		float thirdX = width / 2.0;
		float thirdY = thirdX * tan(secondAngle) + secondB;

		*c1x = max(min(x1 + width / 2.0 + firstX, x1 + width), x1);
		*c1y = y1;

		*c2x = max(min(x1 + width / 2.0 + secondX, x1 + width), x1);
		*c2y = y1 + height;

		*c3x = x1 + width;
		*c3y = max(min(y1 + height / 2.0 + thirdY, y1 + height), y1);
	}
	else if (vecy == 0) {

	if (vecx > 0)
	{
		*c1x = x1 + width;
		*c1y = y1 + height / 2.0;

		*c2x = x1;
		*c2y = y1 + height / 4.0;

		*c3x = x1;
		*c3y = y1 + height * 3.0 / 4.0;
	}
	else if(vecx > 0)
	{
		*c1x = x1;
		*c1y = y1 + height / 2.0;

		*c2x = x1 + width;
		*c2y = y1 + height * 3.0 / 4.0;

		*c3x = x1 + width;
		*c3y = y1 + height / 4.0;
	}
	else if (vecx == 0)
	{
		*c1x = x1 + width / 2.0 +1;
		*c1y = y1 + height / 2.0;

		*c2x = x1 + width / 2.0;
		*c2y = y1 + height / 2.0 + 1;

		*c3x = x1 + width / 2.0 - 1;
		*c3y = y1 + height / 2.0;
	}
		
	}
	else if (vecx == 0)
	{
	if (vecy > 0)
	{
		*c1x = y1 + height;
		*c1y = x1 + width / 2.0;

		*c2x = x1 + width / 4.0;
		*c2y = y1;

		*c3x = x1 + width * 3.0 / 4.0;
		*c3y = y1;
	}
	else
	{
		*c1x = y1;
		*c1y = x1 + width / 2.0;

		*c2x = x1 + width / 4.0;
		*c2y = y1 + height;

		*c3x = x1 + width * 3.0 / 4.0;
		*c3y = y1 + height;
	}
	}
}

// vx ,vy mean the velocity of each vertex.
// div is the result

void divInCell(float vx0, float vy0, float vx1, float vy1, float vx2, float vy2, float vx3, float vy3, float width, float height, float *div) {

	float s = 0.5;
	float r = 0.5;

	//float partialX = ((vx2 - vx1) * s + (vx3 - vx0) * (1 - s)) / width;
	//float partialY = ((vx2 - vx3) * r + (vx1 - vx0) * (1 - r)) / height;

	float partialX = ((sqrt(vx2 * vx2 + vy2 * vy2) - sqrt(vx1 * vx1 + vy1 * vy1)) * s + (sqrt(vx3 * vx3 + vy3 * vy3) - sqrt(vx0 * vx0 + vy0 * vy0)) * (1 - s)) / width;
	float partialY = ((sqrt(vx2 * vx2 + vy2 * vy2) - sqrt(vx3 * vx3 + vy3 * vy3)) * r + (sqrt(vx1 * vx1 + vy1 * vy1) - sqrt(vx0 * vx0 + vy0 * vy0)) * (1 - r)) / height;

	*div = partialX + partialY;
}