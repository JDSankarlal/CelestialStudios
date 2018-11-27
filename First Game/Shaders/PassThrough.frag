#version 420
#define MAX_LIGHTS_SIZE 50
uniform vec4 LightPosition[MAX_LIGHTS_SIZE];

//color
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse[MAX_LIGHTS_SIZE];
uniform vec3 LightSpecular[MAX_LIGHTS_SIZE];

//scalar
uniform float LightSpecularExponent[MAX_LIGHTS_SIZE];
uniform float Attenuation_Constant[MAX_LIGHTS_SIZE];
uniform float Attenuation_Linear[MAX_LIGHTS_SIZE];
uniform float Attenuation_Quadratic[MAX_LIGHTS_SIZE];

//amount of lights
uniform int LightAmount;

//uniform vec3 LightDirection = vec3(0,0,-1);
//uniform float LightAngleConstraint;
uniform sampler2D uTex;
uniform vec4 colourMod;
uniform bool textured;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

//in vec3 normal;

void main()
{
    
    //outColor = vec4(normal, 1.0f);
    //outColor = vec4(0.5f, 1.0f, 0.5f, 1.0f);
    
    if(textured)
    {       
       vec4 textureColor = texture(uTex, texcoord);
       outColor = textureColor;
     //  outColor.rgb *= textureColor.rgb;
    }else     
    {
       outColor = colourMod; 
    }

    outColor.rgb *= LightAmbient;
    
    //account for rasterizer interpolating
    vec3 normal = normalize(norm);
    
    for(int a = 0; a < LightAmount; a++)
    { 
        vec3 lightVec = LightPosition[a].xyz - pos;
        float dist = length(lightVec);
        vec3 lightDir = lightVec / dist;

        float NdotL = dot(normal, lightDir);

        //float angle = acos(dot(lightDir , LightDirection) / length(lightDir * LightDirection));

        if(NdotL > 0.0)
        {
            //The light contributes to this surface
            //Calculate attenuation (falloff)
            float attenuation = 1.0 / (Attenuation_Constant[a] + (Attenuation_Linear[a] * dist) + (Attenuation_Quadratic[a] * dist * dist));
            //Calculate diffuse contribution
            outColor.rgb += LightDiffuse[a] * NdotL * attenuation;
            //Blinn-Phong half vector
            float NdotHV =  max(dot(normal, normalize(lightDir + normalize(-pos))), 0.0); 
            //Calculate specular contribution
            outColor.rgb += LightSpecular[a] * pow(NdotHV, LightSpecularExponent[a]) * attenuation;
        }
     // outColor.rgb = vec3(NdotL,NdotL,NdotL); 
    }
    
}