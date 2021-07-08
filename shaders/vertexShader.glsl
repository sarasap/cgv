#version 150 

in  vec4 vPosition;
in  vec4 vColor;
smooth out vec4 color;

uniform float fcurrentTranslation;
uniform float fcolourCodingOffsetRed;
uniform mat4 model_view;
uniform mat4 projection;

void main() 
{
    gl_Position = projection*model_view*vPosition/vPosition.w;
    color.r = vColor.r * (abs(fcurrentTranslation) + 0.2);
    color.g = vColor.g * (-abs(fcurrentTranslation) + 0.6);
    color.b = vColor.b * (abs(fcurrentTranslation) - 0.4);
	if(fcolourCodingOffsetRed != 0.0)
		color.r = fcolourCodingOffsetRed / 255.0;
} 
