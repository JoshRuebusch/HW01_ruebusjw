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


	static const int appWidth = 1024;
	static const int appHeight = 600;
	static const int appTextureSize = 1024;

	//void drawRectangle(uint8_t pixels, int x1, int x2, int y1, int y2, Color8u fill);

	void drawCircle(uint8_t pixels, int x, int y, int r, Color8u fill);

	void drawLine(uint8_t pixels, int x1, int x2, int y1, int y2, Color8u fill);

	void drawGradient (uint8_t pixels, Color8u startColor, Color8u endColor);
	

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
	
	
};

void HW01_ruebusjwApp::setup()
{
	frameNumber = 0;
	mySurface = new Surface(appTextureSize,appTextureSize,false);

	uint8_t* dataArray = (*mySurface).getData();
	

	for(int i=0; i < (*mySurface).getWidth()*(*mySurface).getHeight(); i++)
		{
			selector = rand() % 1000 + 1;
			if(selector == 1)
			{
				dataArray[3*i] = 255;
				dataArray[3*i+1] = 255;
				dataArray[3*i+2] = 255;
			}
			else
			{
				dataArray[3*i] = 0;
				dataArray[3*i+1] = 0;
				dataArray[3*i+2] = 0;
			}
			
		}
}

void HW01_ruebusjwApp::mouseDown( MouseEvent event )
{
}

void HW01_ruebusjwApp::update()
{
	int startx = 1;
	int endx = 75;
	int starty = 1;
	int endy = 25;
	uint8_t* dataArray = (*mySurface).getData();
	Color8u fill = Color8u(255,0,0);
	drawRectangle(dataArray,startx,endx,starty,endy,fill);
	/*
	startx++;
	endx++;
	starty++;
	endy++;*/
	/*
	for(int i=0; i< (*mySurface).getWidth()*(*mySurface).getHeight(); i++)
	{

	}
	*/
	frameNumber++;
	
}

void HW01_ruebusjwApp::draw()
{
	gl::draw(*mySurface); 
}

CINDER_APP_BASIC( HW01_ruebusjwApp, RendererGl )
