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
	//void prepareSettings(Settings* settings);

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

	void HW01_ruebusjwApp::drawRectangle(uint8_t* pixels, int x1, int x2, int y1, int y2, Color8u fill)
	{
		int startx = (x1 < x2) ? x1 : x2;
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
	
	frameNumber++;
	
}

void HW01_ruebusjwApp::draw()
{
	gl::draw(*mySurface); 
}

CINDER_APP_BASIC( HW01_ruebusjwApp, RendererGl )
