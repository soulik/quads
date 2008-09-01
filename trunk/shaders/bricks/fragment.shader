//
// Fragment shader for procedural bricks
//
// Authors: Dave Baldwin, Steve Koren, Randi Rost
//          based on a shader by Darwyn Peachey
//
// Copyright (c) 2002-2004 3Dlabs Inc. Ltd.
//
//  

/*
uniform vec3  BrickColor, MortarColor;
uniform vec2  BrickSize;
uniform vec2  BrickPct;
  */
uniform int textureId;
varying vec2  MCposition;
varying float LightIntensity;
uniform sampler2D tex;
varying vec4  vertexColor;

void main(void)
{
	vec4 color2 = texture2D(tex,gl_TexCoord[0].st);
	gl_FragColor = color2*vertexColor*LightIntensity;
}
