#version 440 core
layout (location = 0) in vec2 aPos;

out vData {vec3 FragPos;} vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float param_t[10];
uniform int funPlot;


float PI = 3.14159265;
float E = 2.71828182;


int funcionIf(bool cond, int e1, int e2)
{
	if (cond)
		 return e1;
	 else
		 return e2;
}

float funcionIf(bool cond, float e1, float e2)
{
	if (cond)
		 return e1;
	 else
		 return e2;
}

bool funcionIf(bool cond, bool e1, bool e2)
{
	if (cond)
		 return e1;
	 else
		 return e2;
}

vec2 funcionIf(bool cond, vec2 e1, vec2 e2)
{
	if (cond)
		 return e1;
	 else
		 return e2;
}

vec3 funcionIf(bool cond, vec3 e1, vec3 e2)
{
	if (cond)
		 return e1;
	 else
		 return e2;
}

mat2 funcionIf(bool cond, mat2 e1, mat2 e2)
{
	if (cond)
		 return e1;
	 else
		 return e2;
}

mat3 funcionIf(bool cond, mat3 e1, mat3 e2)
{
	if (cond)
		 return e1;
	 else
		 return e2;
}
vec3 f(float u, float v) {
	return  vec3(u - 0.5, v - 0.5, 0.1) * ((u * u) / u);
}

) {
	return  vec3(u - 0.5, v - 0.5, 0.1) * ((u * u) / u) +  vec3(u - 0.5, v - 0.5, 0.1) * ((1 * u + u * 1) / u - (u * u) * 1 / (u * u));
}

void main() {
	vec3 aPosSurf;

	if (funPlot==0) {
		aPosSurf = f(aPos.x, aPos.y);
	}

	vertex.FragPos = vec3(model * vec4(aPosSurf, 1.0));
	gl_Position = projection * view * model * vec4(aPosSurf, 1.0);
}