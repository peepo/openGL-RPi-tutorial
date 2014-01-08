#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "../common/camera.h"
#include "../common/graphics.h"

#define MAIN_TEXTURE_WIDTH 960 //16*60 768 // 16*48    // 704*1024 stretches, provides 6 levels, offsets red one along
#define MAIN_TEXTURE_HEIGHT 640 //16*40 512  // 16*32

// MAIN_TEXTURE is YUV pixels not screen pixels...

#define TEXTURES 2  //how many images or textures?

char tmpbuff[MAIN_TEXTURE_WIDTH*MAIN_TEXTURE_HEIGHT*4];

//entry point
int main(int argc, const char **argv)
{
	//init graphics and the camera
	InitGraphics();
	CCamera* cam = StartCamera(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT,30,1,false);

	//create 4 textures of decreasing size
	GfxTexture ytexture,utexture,vtexture,rgbtextures[TEXTURES],redtexture;

	ytexture.CreateGreyScale(MAIN_TEXTURE_WIDTH,MAIN_TEXTURE_HEIGHT);
	utexture.CreateGreyScale(MAIN_TEXTURE_WIDTH/2,MAIN_TEXTURE_HEIGHT/2);
	vtexture.CreateGreyScale(MAIN_TEXTURE_WIDTH/2,MAIN_TEXTURE_HEIGHT/2);

// every image displayed is a texture, the texture_grid array holds ours
	GfxTexture* texture_grid[TEXTURES];
	memset(texture_grid,0,sizeof(texture_grid));
	int next_texture_grid_entry = 0;
// Background 192,128 is fairly coarse
                rgbtextures[0].CreateRGBA(960,640); // 192,128);
		rgbtextures[0].GenerateFrameBuffer();
		texture_grid[next_texture_grid_entry++] = &rgbtextures[0];
	printf("Running frame loop\n");
	for(int i = 0; i < 3000; i++)
	{

                //spin until we have a camera frame
                const void* frame_data; int frame_sz;
                while(!cam->BeginReadFrame(0,frame_data,frame_sz)) 
                {

                }
		//lock the chosen frame buffer, and copy it directly into the corresponding open gl texture
		{
 			const uint8_t* data = (const uint8_t*)frame_data;
			int ypitch = MAIN_TEXTURE_WIDTH;
			int ysize = ypitch*MAIN_TEXTURE_HEIGHT;
			int uvpitch = MAIN_TEXTURE_WIDTH/2;
			int uvsize = uvpitch*MAIN_TEXTURE_HEIGHT/2;
			//int upos = ysize+16*uvpitch;
			//int vpos = upos+uvsize+4*uvpitch;
			int upos = ysize;
			int vpos = upos+uvsize;
//	printf("Frame data len: 0x%x, ypitch: 0x%x ysize: 0x%x, uvpitch: 0x%x, uvsize: 0x%x, u at 0x%x, v at 0x%x, total 0x%x\n", frame_sz, ypitch, ysize, uvpitch, uvsize, upos, vpos, vpos+uvsize);
			ytexture.SetPixels(data);
			utexture.SetPixels(data+upos);
			vtexture.SetPixels(data+vpos);
			cam->EndReadFrame(0);
		}

		//begin frame, draw the texture then end frame (the bit of maths just fits the image to the screen while maintaining aspect ratio)
		BeginFrame();

		    DrawYUVTextureRect(&ytexture,&utexture,&vtexture,-1.f,-1.f,1.f,1.f,&rgbtextures[0],0);
                    if(GfxTexture* tex = texture_grid[0])
		        DrawTextureRect(tex,-1,-1,1,1,NULL,0,0.0,0.0);

		EndFrame();
	}

	StopCamera();
  return 0;
}

