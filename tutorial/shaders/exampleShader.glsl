#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 real;
uniform vec4 img;
uniform vec4 real_roots;
uniform vec4 img_roots;
uniform int iter;
uniform float scroll;
uniform float x;
uniform float y;

out vec4 Color;

vec2 f(vec2 z) {
	float s = real[0]*(z.x*z.x*z.x-3*z.x*z.y*z.y);
	float t = real[0]*(3*z.x*z.x*z.y-z.y*z.y*z.y);
	vec2 f = vec2(s,t);
	s = real[1]*(z.x*z.x-z.y*z.y);
	t = real[1]*(2*z.x*z.y);
	vec2 k = vec2(s,t);
	vec2 l = vec2(real[2]*z.x, real[2]*z.y);
	return f + k + l + vec2(real[3], 0); 
}

vec2 df(vec2 z){
	float s = 3*real[0]*(z.x*z.x-z.y*z.y);
	float t = 3*real[0]*(2*z.x*z.y);
	vec2 k = vec2(s,t);
	vec2 l = vec2(2*real[1]*z.x, 2*real[1]*z.y);
	return k + l + vec2(real[2], 0);
}

vec2 cx_div(vec2 a, vec2 b) {
	return vec2(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)));}
void main()
{
	vec4[4] colors = vec4[4](vec4(1,0,0,1),vec4(0,1,0,1), vec4(0,0,1,1), vec4(1,1,1,1) );
	vec2 z = scroll*vec2(position0.x-x, position0.y+y);
	for (int i =0; i< iter; i++){
		z = z - cx_div(f(z),df(z));
	}
	float dist0 = distance(z, vec2(real_roots[0],img_roots[0]));
	float dist1 = distance(z, vec2(real_roots[1],img_roots[1]));
	float dist2 = distance(z, vec2(real_roots[2],img_roots[2]));
	float min_dist = min(dist0, min(dist1, dist2));
	if(min_dist == dist0)
		Color = colors[0];
	else if (min_dist == dist1)
		Color = colors[1];
	else if (min_dist == dist2)
		Color = colors[2];
	else	
		Color = colors[3];		 
}

