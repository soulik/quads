varying vec4 temp_pos,temp_pos2;

void main()
{

	gl_TexCoord[0] = gl_MultiTexCoord0;// *gl_MultiTexCoord0;
	gl_Position = ftransform();
	temp_pos =gl_TexCoord[0];
	temp_pos2 =gl_Position;
} 
