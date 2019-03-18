#pragma once
#include <stdbool.h> 

// parameters : x1, y1 the start point of the cell (left, bottom corner)
// width , height: the width and height of the cell
// rho1, rho2, rho3, rho4 : the four density of four vertex
// sepRho : the rho used to make isoline
// return sepx1, spey1, spex2, sepy2. The first pair of coordinations the separate line needs(if there is one)
// return spex3, sepy3, sepx4, sepy4. The second pair of coordinations the separate line needs(if there is one)
// reference : book p170
void linePointsInCell(float x1, float y1, float width, float height,float sepRho,float rho1, float rho2, float rho3, float rho4, float *sepx1, float *sepy1, float *sepx2, float *sepy2, float *sepx3, float *sepy3, float *sepx4, float *sepy4) {

	if ((rho1 > sepRho && rho2 > sepRho && rho3 > sepRho && rho4 > sepRho) || (rho1 < sepRho && rho2 < sepRho && rho3 < sepRho && rho4 < sepRho))
	{ // This case means there is no isoline should be drawn in this cell.
		*sepx1 = 0.0; *sepx2 = 0.0; *sepx3 = 0.0; *sepx4 = 0.0;
		*sepy1 = 0.0; *sepy2 = 0.0; *sepy3 = 0.0; *sepy4 = 0.0;
	}
	
	else if (rho1 >= sepRho && rho2 < sepRho && rho3 < sepRho && rho4 < sepRho) // 0001
	{
		*sepx1 = x1;
		*sepy1 = y1 + height / 2.0;

		*sepx2 = x1 + width / 2.0;
		*sepy2 = y1;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho4 >= sepRho && rho1 < sepRho && rho2 < sepRho && rho3 < sepRho) // 0010
	{
		*sepx1 = x1 + width / 2.0;
		*sepy1 = y1;

		*sepx2 = x1 + width;
		*sepy2 = y1 + height / 2.0;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho4 >= sepRho && rho1 >= sepRho && rho3< sepRho && rho2 < sepRho) // 0011
	{
		*sepx1 = x1;
		*sepy1 = y1 + height / 2.0;

		*sepx2 = x1 + width;
		*sepy2 = y1 + height / 2.0;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho3 >= sepRho && rho1 < sepRho && rho2 < sepRho && rho4 < sepRho) // 0100
	{
		*sepx1 = x1 + width / 2.0;
		*sepy1 = y1 + height;

		*sepx2 = x1 + width;
		*sepy2 = y1 + height / 2.0;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho3 >= sepRho && rho1 >= sepRho && rho2 < sepRho && rho4 < sepRho) // 0101
	{
		*sepx1 = x1;
		*sepy1 = y1 + height / 2.0;

		*sepx2 = x1 + width / 2.0;
		*sepy2 = y1;

		*sepx3 = x1 + width / 2.0;
		*sepy3 = y1 + height;

		*sepx4 = x1 + width;
		*sepy4 = y1 + height / 2.0;
	}
	
	else if (rho3 >= sepRho && rho4 >= sepRho && rho1 < sepRho && rho2 < sepRho) // 0110
	{
		*sepx1 = x1 + width / 2.0;
		*sepy1 = y1;

		*sepx2 = x1 + width / 2.0;
		*sepy2 = y1 + height;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho1 >= sepRho && rho3 >= sepRho && rho4 >= sepRho && rho2 < sepRho) // 0111
	{
		*sepx1 = x1;
		*sepy1 = y1 + height / 2.0;

		*sepx2 = x1 + width / 2.0;
		*sepy2 = y1 + height;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho2 >= sepRho && rho1 < sepRho && rho3 < sepRho && rho4 < sepRho) // 1000
	{
		*sepx1 = x1;
		*sepy1 = y1 + height / 2.0;

		*sepx2 = x1 + width / 2.0;
		*sepy2 = y1 + height;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho1 >= sepRho && rho2 >= sepRho && rho3 < sepRho && rho4 < sepRho) // 1001
	{
		*sepx1 = x1 + width / 2.0;
		*sepy1 = y1;

		*sepx2 = x1 + width / 2.0;
		*sepy2 = y1 + height;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho2 >= sepRho && rho4 >= sepRho && rho1 < sepRho && rho3 < sepRho) // 1010
	{
		*sepx1 = x1;
		*sepy1 = y1 + height / 2.0;

		*sepx2 = x1 + width / 2.0;
		*sepy2 = y1 + height;

		*sepx3 = x1 + width / 2.0;
		*sepy3 = y1;

		*sepx4 = x1 + width;
		*sepy4 = y1 + height / 2.0;
	}
	
	else if (rho1 >= sepRho && rho2 >= sepRho && rho4 >= sepRho && rho3 < sepRho) // 1011
	{ 
		*sepx1 = x1 + width / 2.0;
		*sepy1 = y1 + height;

		*sepx2 = x1 + width;
		*sepy2 = y1 + height / 2.0;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho2 >= sepRho && rho3 >= sepRho && rho1 < sepRho && rho4 < sepRho) // 1100
	{
		*sepx1 = x1;
		*sepy1 = y1 + height / 2.0;
	
		*sepx2 = x1 + width;
		*sepy2 = y1 + height / 2.0;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
	else if (rho1 >= sepRho && rho2 >= sepRho && rho3 >= sepRho && rho4 < sepRho) // 1101
	{
		*sepx1 = x1 + width / 2.0;
		*sepy1 = y1;

		*sepx2 = x1 + width;
		*sepy2 = y1 + height / 2.0;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	else if (rho2 >= sepRho && rho3 >= sepRho && rho4 >= sepRho && rho1 < sepRho) // 1110
	{

		*sepx1 = x1;
		*sepy1 = y1 + height / 2.0;

		*sepx2 = x1 + width / 2.0;
		*sepy2 = y1;

		*sepx3 = 0; *sepx4 = 0;
		*sepy3 = 0; *sepy4 = 0;
	}
	
}

void multiLinesPointsInCell(float startX, float startY, float rho1, float rho2, float rho3, float rho4,float *rhoValues) {

	int n = sizeof(rhoValues);

	int leftbottom = 0;
	int bottmright = 0;
	int rightupper = 0;
	int upperleft = 0;
	int horizen = 0;
	int vertical = 0;

	for (size_t i = 0; i < n; i++)
	{
		float val = rhoValues[i];

		if ((rho1 > val && rho2 > val && rho3 > val && rho4 > val) || (rho1 < val && rho2 < val && rho3 < val && rho4 < val)) {

		}
		else if (rho1 >= val && rho2 < val && rho3 < val && rho4 < val) // 0001
		{

		}
		else if (rho4 >= val && rho1 < val && rho2 < val && rho3 < val) // 0010
		{

		}
		else if (rho4 >= val && rho1 >= val && rho3 < val && rho2 < val) // 0011
		{

		}
		else if (rho3 >= val && rho1 < val && rho2 < val && rho4 < val) // 0100
		{

		}
		else if (rho3 >= val && rho1 >= val && rho2 < val && rho4 < val) // 0101
		{

		}
		else if (rho3 >= val && rho4 >= val && rho1 < val && rho2 < val) // 0110
		{

		}
		else if (rho1 >= val && rho3 >= val && rho4 >= val && rho2 < val) // 0111
		{

		}
		else if (rho2 >= val && rho1 < val && rho3 < val && rho4 < val) // 1000
		{

		}
		else if (rho1 >= val && rho2 >= val && rho3 < val && rho4 < val) // 1001
		{

		}
		else if (rho2 >= val && rho4 >= val && rho1 < val && rho3 < val) // 1010
		{

		}
		else if (rho1 >= val && rho2 >= val && rho4 >= val && rho3 < val) // 1011
		{

		}
		else if (rho2 >= val && rho3 >= val && rho1 < val && rho4 < val) // 1100
		{

		}
		else if (rho1 >= val && rho2 >= val && rho3 >= val && rho4 < val) // 1101
		{

		}
		else if (rho2 >= val && rho3 >= val && rho4 >= val && rho1 < val) // 1110
		{

		}
	}
}