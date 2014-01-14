openGLES 2.0 tutorial for Raspberry Pi

sudo apt-get install git cmake libglm-dev

git clone https://github.com/raspberrypi/userland.git

git clone https://github.com/peepo/openGL-RPi-tutorial.git

cd userland

mkdir build

cd build

cmake ../

make

cd ../../

sudo mv userland /opt/vc

cd openGL-RPi-tutorial/build

cmake ../

make

cd ../tutorial01_first_screen/

./tutorial01_first_screen

---

start with tutorial01 to 06 then encode, and finally encode_OGL please send comments, and report bugs to the current maintainer: Jonathan Chetwynd

jay@peepo.com

created in conjunction with these excellent resources:

openGL tutorials 1-5: http://www.opengl-tutorial.org/

openGLES 2.0 Programmers Guide 2008, Raspberry Pi code port: http://benosteen.wordpress.com/2012/04/27/using-opengl-es-2-0-on-the-raspberry-pi-without-x-windows/

Chris Cummings' My Robot Blog http://robotblogging.blogspot.co.uk/2013/10/pi-eyes-stage-1.html

drhastings' robots: https://github.com/drhastings/balance
  
and videos: http://www.youtube.com/user/drhastings90

okay so I lifted the code, so blame me not them....

my intention is to further understanding, by using only the minimum necessary code. Unfortunately at this time I failed rather badly

~:"
