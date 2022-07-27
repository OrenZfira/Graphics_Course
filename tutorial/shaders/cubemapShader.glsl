#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;
in vec3 lookat; // direction vector representing a 3D texture coordinate

uniform vec4 lightColor;
uniform samplerCube sampler1;
uniform vec4 lightDirection;
uniform int fog;

out vec4 Color;
void main()
{
	vec4  fog_colour = vec4(0.6, 0.6, 0.6, 0);

	Color = mix(vec4(texture(sampler1, lookat).xyz,0), fog_colour, 0.8*fog);
}  


