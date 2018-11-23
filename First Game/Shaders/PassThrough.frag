#version 420

uniform vec4 LightPosition;

//color
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;

//scalar
uniform float LightSpecularExponent;
uniform float Attenuation_Constant;
uniform float Attenuation_Linear;
uniform float Attenuation_Quadratic;

uniform vec3 LightDirection = vec3(0,0,-1);
uniform float LightAngleConstraint;
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

    
    //account for rasterizer interpolating
    vec3 normal = normalize(norm);
    
    vec3 lightVec = LightPosition.xyz - pos;
    float dist = length(lightVec);
    vec3 lightDir = lightVec / dist;
    
    float NdotL = dot(normal, lightDir);

    float angle = acos(dot(lightDir , LightDirection) / length(lightDir * LightDirection));

    if(LightAngleConstraint < angle)
        if(NdotL > 0.0)
        {
            //The light contributes to this surface

            //Calculate attenuation (falloff)
            float attenuation = 1.0 / (Attenuation_Constant + (Attenuation_Linear * dist) + (Attenuation_Quadratic * dist * dist));

            //Calculate diffuse contribution
            outColor.rgb += LightDiffuse * NdotL * attenuation;

            //Blinn-Phong half vector
            float NdotHV =  max(dot(normal, normalize(lightDir + normalize(-pos))), 0.0); 

            //Calculate specular contribution
            outColor.rgb += LightSpecular * pow(NdotHV, LightSpecularExponent) * attenuation;
        }
   // outColor.rgb = normal; 
}