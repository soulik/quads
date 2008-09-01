varying vec3 T,B,N;
uniform vec4 col;

void main(void)
{
gl_FragColor =  vec4(N/2.0 + 0.5,1.0) + col;
//gl_FragColor = 0.1 + 0.9 *T.x;

}