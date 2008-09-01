/*
uniform sampler2D tex0;

void main(void)
{
	vec4 blurSample = 0.0;
	vec4 tmpPix;
	vec4 offPix;
	vec2 uv = gl_TexCoord[0].st;
	for(int i=-4;i<5;i++)
	{
		tmpPix = texture2D(tex0,uv + vec2( i*0.005,0 ));
		offPix = -0.3+tmpPix;
		offPix = offPix * 15;
		if( (offPix.r+offPix.g+offPix.b)>0 )
		{			
                blurSample = blurSample + offPix;	
		}
	}

	for(int i=-4;i<5;i++)
	{
		tmpPix = texture2D(tex0,uv + vec2( 0,i*0.005 ));
              	offPix = -0.3+tmpPix;
		offPix = offPix * 15;
		if( (offPix.r+offPix.g+offPix.b)>0 )
		{
			blurSample += offPix;
		}

	}

	blurSample = blurSample / 64;
	gl_FragColor = blurSample*1.2;
}
*/

/*
uniform sampler2D texture0_2D;
uniform float ddx;
uniform float ddy;

void main(void)
{
  //Initialisierungen
  vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);

  // Texturen auslesen
  // und vertikal bluren (gauss)
  outp += 0.015625 * texture2D(texture0_2D, gl_TexCoord[0].xy + vec2(ddx*-3.0, 0.0) );
  outp += 0.09375 * texture2D(texture0_2D, gl_TexCoord[0].xy + vec2(ddx*-2.0, 0.0) );
  outp += 0.234375 * texture2D(texture0_2D, gl_TexCoord[0].xy + vec2(ddx*-1.0, 0.0) );
  outp += 0.3125 * texture2D(texture0_2D, gl_TexCoord[0].xy + vec2(0.0, 0.0) );
  outp += 0.234375 * texture2D(texture0_2D, gl_TexCoord[0].xy + vec2(ddx*1.0, 0.0) );
  outp += 0.09375 * texture2D(texture0_2D, gl_TexCoord[0].xy + vec2(ddx*2.0, 0.0) );
  outp += 0.015625 * texture2D(texture0_2D, gl_TexCoord[0].xy + vec2(ddx*3.0, 0.0) );

  gl_FragColor =  outp;
 }

*/

/*
uniform sampler2D texture0_2D;
uniform vec4 Contrast;
uniform vec4 Brightness;

vec4 saturate(vec4 inp)
{
  return clamp(inp, 0.0, 1.0);
}

void main(void)
{
  //Initialisierungen
  vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);

  // Texturen auslesen
  outp = texture2D(texture0_2D, gl_TexCoord[0].xy );

  //Kontrast
  vec4 c = vec4(2.0,2.0,2.0,1.0) * (vec4(1.0,1.0,1.0,1.0) - Contrast);
  outp = saturate((outp - vec4(0.5,0.5,0.5,0.5)) * c + vec4(0.5,0.5,0.5,0.5));

  //Farben pushen
  outp *= Brightness;

  //Helligkeit
  vec4 b = (vec4(1.0,1.0,1.0,1.0) - Contrast * vec4(2.0,2.0,2.0,1.0));
  outp = saturate(outp + b);

  gl_FragColor =  outp;
 }

*/
/*
varying float intensity;
	
void main()
{
	vec4 color;
	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);
	gl_FragColor = color;
} 
*/

uniform sampler2D tex;
varying vec4 temp_pos, temp_pos2;

void main(){
	vec4 color1 = texture2D(tex,gl_TexCoord[0].st);
	vec4 color2 = gl_Color;
	vec4 color = color1;
	//gl_FragColor = color*temp_pos2*vec4(1.0,0.2,0,0.5)+vec4(1.0,0.2,0,1);
	color = color-((float(temp_pos2[3])-20.0)/200.0);//+vec4(0.5,0.1,0,0));
	gl_FragData[0] = color;//-gl_FragData[0];
}


/*
bool greater (in vec2 v1, in vec2 v2){
	return all(greaterThan(v1,v2));
}

uniform vec2 threshold;
uniform vec3 color;
bool greater(in vec2 v1, in vec2 v2);

void main(){
	vec2 ss = fract(gl_TexCoord[0].st*10.0);
	if (greater(ss, threshold))
		discard;
	gl_FragColor = vec4(color,1.0);
}

*/
