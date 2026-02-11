#version 450 core
layout(std430, binding = LUM_SSBO_LIGHTS_BINDING) buffer Lights {
	PointLight lights[];
}