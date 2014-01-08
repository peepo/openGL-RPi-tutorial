attribute vec4 vertex;
uniform vec2 offset;
uniform vec2 scale;
varying vec2 tcoord;
void main(void) 
{
	vec4 pos = vertex;
	tcoord.xy = pos.xy;
//        tcoord.st = vec2(pos.x, 1.0 - pos.y);
	pos.xy = pos.xy*scale+offset;
	gl_Position = pos;
}
