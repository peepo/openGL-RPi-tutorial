#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "../common/camera.h"
#include "../common/cam_graphics.h"

#define MAIN_TEXTURE_WIDTH 960 //16*60 768 // 16*48    // 704*1024 stretches, provides 6 levels, offsets red one along
#define MAIN_TEXTURE_HEIGHT 640 //16*40 512  // 16*32

// MAIN_TEXTURE is YUV pixels not screen pixels...

#define TEXTURES 2  //how many images or textures?

   int framenumber = 0;
char tmpbuff[MAIN_TEXTURE_WIDTH*MAIN_TEXTURE_HEIGHT*4];

//entry point
int main(int argc, const char **argv)
{

	//init graphics 
	InitGraphics();

	//create 4 textures of decreasing size
	GfxTexture ytexture,utexture,vtexture,rgbtextures[TEXTURES];

	ytexture.CreateGreyScale(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	utexture.CreateGreyScale(MAIN_TEXTURE_WIDTH/2,MAIN_TEXTURE_HEIGHT/2);
	vtexture.CreateGreyScale(MAIN_TEXTURE_WIDTH/2,MAIN_TEXTURE_HEIGHT/2);

// every image displayed is a texture, the texture_grid array holds ours
	GfxTexture* texture_grid[TEXTURES];
	memset(texture_grid,0,sizeof(texture_grid));
	int next_texture_grid_entry = 0;
// Background 192,128 is fairly coarse
                rgbtextures[0].CreateRGBA(240,160); // 192,128);
		rgbtextures[0].GenerateFrameBuffer();
		texture_grid[next_texture_grid_entry++] = &rgbtextures[0];
	printf("Running frame loop\n");
	for(int i = 0; i < 3000; i++)
	{
                //spin until we have a camera frame
                // removed

   int i, j, uWidth=(MAIN_TEXTURE_WIDTH >> 1), Pitch=MAIN_TEXTURE_WIDTH*MAIN_TEXTURE_HEIGHT, frameno = framenumber++;
   uint8_t data[Pitch*3/2];
   uint8_t *y = (uint8_t*)data, *u = y + Pitch, *v =
      u + (Pitch >> 2); // ie PITCH*HEIGHT16*1/4   

   for (j = 0; j < MAIN_TEXTURE_HEIGHT / 2; j++) {
      uint8_t *py = y + 2 * j * MAIN_TEXTURE_WIDTH;
      uint8_t *pu = u + j * uWidth;  // PITCH / 2
      uint8_t *pv = v + j * uWidth;
      for (i = 0; i < uWidth; i++) {
         int z = (((i + frameno) >> 3) ^ (j >> 4)) & 15; // defines colour changes, i + frame moves to left, 3 or 4 set frequency of$
         py[0] = py[1] = py[MAIN_TEXTURE_WIDTH] = py[MAIN_TEXTURE_WIDTH + 1] = 0x80 + z * 0x8;
         pu[0] = 0x00 + z * 0x10;
         pv[0] = 0x80 + z * 0x30;
         py += 2;
         pu++;
         pv++;
      }
   }
                        ytexture.SetPixels(y);
                        utexture.SetPixels(u);
                        vtexture.SetPixels(v);

		//begin frame, draw the texture then end frame (the bit of maths just fits the image to the screen while maintaining aspect ratio)
		BeginFrame();

                    DrawYUVTextureRect(&ytexture,&utexture,&vtexture,-1.f,-1.f,1.f,1.f,&rgbtextures[0],0);
                    if(GfxTexture* tex = texture_grid[0])  
                        DrawTextureRect(tex,-1,-1,1,1,NULL,0,0.0,0.0); // width and height set = 0.25 in graphics.cpp

		EndFrame();
	}

  return 0;
}

