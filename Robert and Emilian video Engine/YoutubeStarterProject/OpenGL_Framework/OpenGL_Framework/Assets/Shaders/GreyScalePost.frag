#version 420

uniform sampler2D uTex;

in vec2 texcoord;

out vec4 outColor;

void main()
{
    
    vec4 source = texture(uTex, texcoord); 

	float lumiance = 0.2989f * source.r + 0.587f * source.g + 0.114f * source.b;

	outColor.rgb = vec3(lumiance, lumiance, lumiance);
	outColor.a = 1.0f;
}