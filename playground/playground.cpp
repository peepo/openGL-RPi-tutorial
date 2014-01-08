#include <stdio.h>
#include <unistd.h>
#include "../common/startScreen.h"

void DrawWhiteRect(float x0, float y0, float x1, float y1)
{

}

int main(int argc, const char **argv)
{
        InitGraphics();
        printf("Screen started\n");
        updateScreen();
        printf("Screen is updating\n");
        while(1){}
}

