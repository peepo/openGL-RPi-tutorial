varying vec2 tcoord;
uniform sampler2D tex;
void main(void) 
{
    gl_FragColor = texture2D(tex,tcoord);
}
