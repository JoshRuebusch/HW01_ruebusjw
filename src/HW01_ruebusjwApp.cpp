/*Joshua Ruebusch
CSE 274 HW01
This program draws a picture with two rectangles that move and the ability to make a gradient appear and dissappear by left and right clicking.
Pictures are draw using row major order and accessing the pixel array directly. 
*/


#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HW01_ruebusjwApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
	Surface* mySurface;
	
	int frameNumber;
	int selector;
	int startx1;
	int endx1;
	int starty1;
	int endy1;
	int startx2, starty2, endx2, endy2;
	bool down;


	static const int appWidth = 800;
	static const int appHeight = 600;
	static const int appTextureSize = 1024;

	//void drawRectangle(uint8_t pixels, int x1, int x2, int y1, int y2, Color8u fill);

	void drawCircle(uint8_t pixels, int x, int y, int r, Color8u fill);

	void drawLine(uint8_t pixels, int x1, int x2, int y1, int y2, Color8u fill);

	//void drawGradient(uint8_t pixels, Color8u startColor, Color8u endColor);
	

	void HW01_ruebusjwApp::prepareSettings(Settings* settings)
	{
		(*settings).setWindowSize(appWidth,appHeight);
		(*settings).setResizable(false);
	}

	//rectangle completes A.1
	void HW01_ruebusjwApp::drawRectangle(uint8_t* pixels, int x1, int x2, int y1, int y2, Color8u fill)
	{
		int startx = (x1 < x2) ? x1 : x2;		//This bit is from Dr. Brinkman's rectangle method, rest is my own code
		int endx = (x1 < x2) ? x2 : x1;
		int starty = (y1 < y2) ? y1 : y2;
		int endy = (y1 < y2) ? y2 : y1;

		//fill = Color8u(255,0,0);
		bool isPartOfRect = false;
		int count = 0;
		int pixelCount = 0;

		int length = (endx - startx) + 1;
		int width = (endy - starty) + 1;

		for(int i = 0; i < appWidth*appHeight; i++)
		{
			if((i >= 3*(startx + starty*appTextureSize)) && (i <= 3*(endx + endy*appTextureSize)))
			{
				isPartOfRect = true;
			}
			else 
				isPartOfRect = false;

			while(isPartOfRect)
			{
				pixels[3*i] = fill.r;
				pixels[3*i+1] = fill.g;
				pixels[3*i+2] = fill.b;
				pixelCount++;
				i++;
				if(pixelCount>length)
				{
					i = (i + appTextureSize - length) - 2;
					pixelCount = 0;
					isPartOfRect = false;
				}
			}
		}
	}

	//Completes A.4 to make a gradient
	void HW01_ruebusjwApp::drawGradient(uint8_t* pixels, Color8u startColor, Color8u endColor)
	{
		int count = 0;
		for(int i = 0; i < appTextureSize*appTextureSize; i++)
		{
		    if(count>=appTextureSize)
			{
				startColor.g = 255;
				count = 0;
			}

			pixels[3*i] = startColor.r;
			pixels[3*i+1] = startColor.g;
			pixels[3*i+2] = startColor.b;
			count++;

			if(count%3 == 0)
			{
				startColor.g--;
			}

		}

	}

	//This was attempt at blur method. Tried, and did not work as expected. Did not have the time to rectify
	//Possible overflow or perhaps accessing improper data
	void HW01_ruebusjwApp::blurImage(uint8_t* pixels)
	{
		Color8u colorSum = Color8u(0,0,0);
		for(int i = 0; i < appTextureSize*appTextureSize; i++)
		{
			colorSum.r = pixels[(3*i)]+pixels[(3*i)+1]+pixels[(3*i)-1]+pixels[(3*i)+appTextureSize]
				+pixels[(3*i)+appTextureSize+1]+pixels[(3*i)+appTextureSize-1]+ pixels[(3*i)-appTextureSize]
				+pixels[(3*i)-appTextureSize-1]+pixels[(3*i)-appTextureSize+1];

			colorSum.g = pixels[(3*i+1)]+pixels[(3*i+1)+1]+pixels[(3*i+1)-1]+pixels[(3*i+1)+appTextureSize]+
				pixels[(3*i+1)+appTextureSize+1]+pixels[(3*i+1)+appTextureSize-1]+pixels[(3*i+1)-appTextureSize]+ 
				pixels[(3*i+1)-appTextureSize+1]+pixels[(3*i+1)-appTextureSize-1];

			colorSum.b = pixels[(3*i+2)]+pixels[(3*i+2)+1]+pixels[(3*i+2)-1]+pixels[(3*i+2)+appTextureSize]+
				pixels[(3*i+2)+appTextureSize+1]+pixels[(3*i+2)+appTextureSize-1]+pixels[(3*i+2)-appTextureSize]+ 
				pixels[(3*i+2)-appTextureSize+1]+pixels[(3*i+2)-appTextureSize-1];

			colorSum.r = colorSum.r/9;
			colorSum.g = colorSum.g/9;
			colorSum.b = colorSum.b/9;

			pixels[3*i] = colorSum.r;
			pixels[3*i+1] = colorSum.g;
			pixels[3*i+2] = colorSum.b;

		}

	}

	

};



void HW01_ruebusjwApp::setup()
{
	frameNumber = 0;
	startx1 = 0;
	endx1 = 800;
	starty1 = 120;
	endy1 = 140;

	startx2 = 0;
	endx2 = 800;
	starty2 = 90;
	endy2 = 70;

	down=false;

	mySurface = new Surface(appTextureSize,appTextureSize,false);

	uint8_t* dataArray = (*mySurface).getData();
	

	for(int i=0; i < (*mySurface).getWidth()*(*mySurface).getHeight(); i++)
		{
			{
				dataArray[3*i] = 255;
				dataArray[3*i+1] = 255;
				dataArray[3*i+2] = 255;
			}
		}
}

void HW01_ruebusjwApp::mouseDown( MouseEvent event )
{
	//Mouse interaction (maybe not that interesting) completes E.6
	Color8u startGrad = Color8u(0,0,0);
	Color8u endGrad = Color8u(0,255,0);
	uint8_t* dataArray = (*mySurface).getData();

	if(event.isLeftDown())
		down = true;
	if(event.isLeftDown()==false)
		down = false;
}

void HW01_ruebusjwApp::update()
{
	//Use of update draw loop completes E.5
	uint8_t* dataArray = (*mySurface).getData();
	Color8u fill = Color8u(0,0,255);
	Color8u fill2 = Color8u(255,0,0);

	Color8u startGrad = Color8u(0,0,0);
	Color8u endGrad = Color8u(0,255,0);

	
	if(down)
	{
		drawGradient(dataArray,startGrad,endGrad);
	}
	else
	{
		for(int i=0; i < (*mySurface).getWidth()*(*mySurface).getHeight(); i++)
		{
			{
				dataArray[3*i] = 255;
				dataArray[3*i+1] = 255;
				dataArray[3*i+2] = 255;
			}
		}
	}

	
	startx1++;
	endx2=endx2-3;

	drawRectangle(dataArray,startx1,endx1,starty1,endy1,fill);
	drawRectangle(dataArray,startx2,endx2,starty2,endy2,fill2);
	/*
	if (frameNumber>5)
		blurImage(dataArray);*/
	//Blur does cause a distortion in the image, but not an expected one
	frameNumber++;
	
}

void HW01_ruebusjwApp::draw()
{
	gl::draw(*mySurface); 
}

CINDER_APP_BASIC( HW01_ruebusjwApp, RendererGl )
