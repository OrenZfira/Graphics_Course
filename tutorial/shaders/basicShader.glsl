#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;
in vec3 position1;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;

out vec4 Color;
void main()
{
	vec3 posAbs  = abs(position1);
    vec3 color   = step(posAbs.yzx, posAbs) * step(posAbs.zxy, posAbs); 
    color       += (1.0 - step(color.zxy * position1.zxy, vec3(0.0)));

	gl_FragColor = texture2D(sampler1, texCoord0)*vec4(color, 1.0);
}
