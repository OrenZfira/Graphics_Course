#version 330

attribute vec3 position;
attribute vec3 normal;
attribute vec4 Ka;
attribute vec4 Kd;
attribute vec4 Ks;
attribute vec2 texcoord;

out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;
out vec3 position0;
out vec3 position1;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	position1 = position;
	texCoord0 = texcoord;
	color0 = vec3(abs(normal));//vec3(Ka);
	if(normal.x < 0)
		color0.y = 1.0;
	if(normal.y < 0)
		color0.z = 1.0;
	if(normal.z < 0)
		color0.x = 1.0;
	normal0 = (Model  * vec4(normal, 0.0)).xyz;
	position0 = vec3(Proj *View *Model * vec4(position, 1.0));
	gl_Position = Proj *View * Model* vec4(position, 1.0); //you must have gl_Position
}
