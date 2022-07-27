#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;
uniform int fog;


void main()
{
	float fog_max = 20.0;
	float fog_min = 0.1;
	vec4  fog_colour = vec4(0.6, 0.6, 0.6, 0);

	// Calculate fog
	float fog_factor = (fog_max - length(position0.xyz)) /
					(fog_max - fog_min);
	fog_factor = clamp(fog_factor, 0.2, 1);
	fog_factor = fog_factor*fog_factor;

	gl_FragColor = mix(vec4(texture2D(sampler1, texCoord0).xyz,0), fog_colour, (1-fog_factor)*fog); //you must have gl_FragColor
}

