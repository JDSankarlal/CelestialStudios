#version 420

uniform sampler2D uTex;
uniform float uPixleSize;// 1 / width
in vec2 outUV;

out vec3 outColour;

void main()
{
    outColour = vec3(0,0,0);

    outColour += texture(uTex,vec2(outUV.x,outUV.y - 3 * uPixleSize)).rgb * 0.09;
    outColour += texture(uTex,vec2(outUV.x,outUV.y - 2 * uPixleSize)).rgb * 0.12;
    outColour += texture(uTex,vec2(outUV.x,    outUV.y - uPixleSize)).rgb * 0.15;
    outColour += texture(uTex,vec2(outUV.x,                 outUV.y)).rgb * 0.16;
    outColour += texture(uTex,vec2(outUV.x,    outUV.y + uPixleSize)).rgb * 0.15;
    outColour += texture(uTex,vec2(outUV.x,outUV.y + 2 * uPixleSize)).rgb * 0.12;
    outColour += texture(uTex,vec2(outUV.x,outUV.y + 3 * uPixleSize)).rgb * 0.09;
}