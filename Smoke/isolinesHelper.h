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

void multiLinesPointsInCell(float startX, float startY, float width, float height, float rho1, float rho2, float rho3, float rho4,float *rhoValues) {

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
			leftbottom++;
		}
		else if (rho4 >= val && rho1 < val && rho2 < val && rho3 < val) // 0010
		{
			bottmright++;
		}
		else if (rho4 >= val && rho1 >= val && rho3 < val && rho2 < val) // 0011
		{
			horizen++;
		}
		else if (rho3 >= val && rho1 < val && rho2 < val && rho4 < val) // 0100
		{
			rightupper++;
		}
		else if (rho3 >= val && rho1 >= val && rho2 < val && rho4 < val) // 0101
		{
			leftbottom++;
			rightupper++;
		}
		else if (rho3 >= val && rho4 >= val && rho1 < val && rho2 < val) // 0110
		{
			vertical++;
		}
		else if (rho1 >= val && rho3 >= val && rho4 >= val && rho2 < val) // 0111
		{
			upperleft++;
		}
		else if (rho2 >= val && rho1 < val && rho3 < val && rho4 < val) // 1000
		{
			upperleft++;
		}
		else if (rho1 >= val && rho2 >= val && rho3 < val && rho4 < val) // 1001
		{
			vertical++;
		}
		else if (rho2 >= val && rho4 >= val && rho1 < val && rho3 < val) // 1010
		{
			upperleft++;
			bottmright++;
		}
		else if (rho1 >= val && rho2 >= val && rho4 >= val && rho3 < val) // 1011
		{
			rightupper++;
		}
		else if (rho2 >= val && rho3 >= val && rho1 < val && rho4 < val) // 1100
		{
			horizen++;
		}
		else if (rho1 >= val && rho2 >= val && rho3 >= val && rho4 < val) // 1101
		{
			bottmright++;
		}
		else if (rho2 >= val && rho3 >= val && rho4 >= val && rho1 < val) // 1110
		{
			leftbottom++;
		}
	}

	int leftbottom_used = 0;
	int bottmright_used = 0;
	int rightupper_used = 0;
	int upperleft_used = 0;
	int horizen_used = 0;
	int vertical_used = 0;

	for (size_t i = 0; i < n; i++)
	{
		float sepRho = rhoValues[i];

		if ((rho1 > sepRho && rho2 > sepRho && rho3 > sepRho && rho4 > sepRho) || (rho1 < sepRho && rho2 < sepRho && rho3 < sepRho && rho4 < sepRho))
		{ // This case means there is no isoline should be drawn in this cell.
	
		}
		else if (rho1 >= sepRho && rho2 < sepRho && rho3 < sepRho && rho4 < sepRho) // 0001
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX, startY + (height / 2.0) * (leftbottom_used + 1) / (float)(leftbottom + 1.0));
			glVertex2f(startX + (width / 2.0) * (leftbottom_used + 1) / (float)(leftbottom + 1.0), startY);
			leftbottom_used++;
		}
		else if (rho4 >= sepRho && rho1 < sepRho && rho2 < sepRho && rho3 < sepRho) // 0010
		{
			glColor3f(0.0, 1.0, 0.0);
			glVertex2f(startX + (width / 2.0) * (bottmright_used + 1) / (bottmright + 1.0), startY);
			glVertex2f(startX + width, startY + (height / 2.0) * (bottmright_used + 1.0) / (bottmright + 1.0));
			bottmright_used++;
		}
		else if (rho4 >= sepRho && rho1 >= sepRho && rho3 < sepRho && rho2 < sepRho) // 0011
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX, startY + (height / (float)horizen + 1.0) * horizen_used);
			glVertex2f(startX + width, startY + (height / (float)horizen + 1.0) * horizen_used);
			horizen_used++;
		}
		else if (rho3 >= sepRho && rho1 < sepRho && rho2 < sepRho && rho4 < sepRho) // 0100
		{
			glColor3f(0.0, 0.0, 1.0);
			glVertex2f(startX + (width / 2.0) * (rightupper_used + 1) / (rightupper + 1.0), startY + height);
			glVertex2f(startX + width, startY + (height / 2.0) * (rightupper_used + 1.0) / (rightupper + 1.0));
			rightupper_used++;
		}
		else if (rho3 >= sepRho && rho1 >= sepRho && rho2 < sepRho && rho4 < sepRho) // 0101
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX, startY + (height / 2.0) * (leftbottom_used + 1.0) / (leftbottom + 1.0));
			glVertex2f(startX +(width / 2.0) * (leftbottom_used + 1) / (leftbottom + 1.0), startY);
			leftbottom_used++;

			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX + (width / 2.0) * (rightupper_used + 1) / (rightupper + 1.0), startY + height);
			glVertex2f(startX + width, startY + (height / 2.0) * (rightupper_used + 1.0) / (rightupper + 1.0));
			rightupper_used++;
		}
		else if (rho3 >= sepRho && rho4 >= sepRho && rho1 < sepRho && rho2 < sepRho) // 0110
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX + vertical_used * width / (vertical + 1), startY);
			glVertex2f(startX + vertical_used * width / (vertical + 1), startY + height);
			vertical_used++;
		}
		else if (rho1 >= sepRho && rho3 >= sepRho && rho4 >= sepRho && rho2 < sepRho) // 0111
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX, startY + (height / 2.0) * (upperleft_used + 1.0) / (upperleft + 1.0));
			glVertex2f(startX + (width / 2.0) * (upperleft_used + 1) / (upperleft + 1.0), startY + height);
			upperleft_used++;
		}
		else if (rho2 >= sepRho && rho1 < sepRho && rho3 < sepRho && rho4 < sepRho) // 1000
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX, startY + (height / 2.0) * (upperleft_used + 1.0) / (upperleft + 1.0));
			glVertex2f(startX + (width / 2.0) * (upperleft_used + 1) / (upperleft + 1.0), startY + height);
			upperleft_used++;
		}

		else if (rho1 >= sepRho && rho2 >= sepRho && rho3 < sepRho && rho4 < sepRho) // 1001
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX + width * (vertical_used + 1) / (vertical + 1.0), startY);
			glVertex2f(startX + width * (vertical_used + 1) / (vertical + 1.0), startY + height);
			vertical_used++;
		}

		else if (rho2 >= sepRho && rho4 >= sepRho && rho1 < sepRho && rho3 < sepRho) // 1010
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX, startY + (height / 2.0) * (upperleft_used + 1.0) / (upperleft + 1.0));
			glVertex2f(startX + (width / 2.0) * (upperleft_used + 1) / (upperleft + 1.0), startY + height);
			upperleft_used++;

			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX + (width / 2.0) * (bottmright_used + 1) / (bottmright + 1.0), startY);
			glVertex2f(startX + width, startY + (height / 2.0) * (bottmright_used + 1.0) / (bottmright + 1.0));
			bottmright_used++;
		}
		else if (rho1 >= sepRho && rho2 >= sepRho && rho4 >= sepRho && rho3 < sepRho) // 1011
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX + (width / 2.0) * (rightupper_used + 1) / (rightupper + 1.0), startY + height);
			glVertex2f(startX + width, startY + (height / 2.0) * (rightupper_used + 1.0) / (rightupper + 1.0));
			rightupper_used++;
		}
		else if (rho2 >= sepRho && rho3 >= sepRho && rho1 < sepRho && rho4 < sepRho) // 1100
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX, startY + (height / (float)horizen + 1.0) * horizen_used);
			glVertex2f(startX + width, startY + (height / (float)horizen + 1.0) * horizen_used);
			horizen_used++;
		}

		else if (rho1 >= sepRho && rho2 >= sepRho && rho3 >= sepRho && rho4 < sepRho) // 1101
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(startX + (width / 2.0) * (bottmright_used + 1) / (bottmright + 1.0), startY);
			glVertex2f(startX + width, startY + (height / 2.0) * (bottmright_used + 1.0) / (bottmright + 1.0));
			bottmright_used++;
		}
		else if (rho2 >= sepRho && rho3 >= sepRho && rho4 >= sepRho && rho1 < sepRho) // 1110
		{

		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(startX, startY + (height / 2.0) * (leftbottom_used + 1.0) / (leftbottom + 1.0));
		glVertex2f(startX + (width / 2.0) * (leftbottom_used + 1) / (leftbottom + 1.0), startY);
		leftbottom_used++;
		}
	}
}