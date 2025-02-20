#version 440
layout (triangles) in;
layout (triangle_strip, max_vertices = 256) out;

in vData {vec3 FragPos; vec3 Normal; float Area; vec2 Coord;} vertices[];

out fData {vec3 FragPos; vec3 Normal; float Area;} frag;

uniform mat4 model;
uniform mat4 tr_inv_model;
uniform mat4 view;
uniform mat4 projection;

uniform float umbralArea;
uniform float param_t[10];
uniform int funPlot;
