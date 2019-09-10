#version 420
#define MAX_LIGHTS_SIZE 50

//
const int DEFAULT=0;
const int POINT=1;
const int DIRECTIONAL=2;
const int SPOTLIGHT=3;


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

uniform bool LightEnable[MAX_LIGHTS_SIZE];
uniform int LightType[MAX_LIGHTS_SIZE];
uniform vec3 LightDirection[MAX_LIGHTS_SIZE] ;
uniform float LightAngleConstraint[MAX_LIGHTS_SIZE];


//uniform sampler2D uTex;
uniform sampler2D uPos;
uniform sampler2D uNorm;
uniform sampler2D uScene;
uniform sampler2D uRamp;

uniform bool toonActive;

in vec2 texcoord;

out vec4 outColor;

void pointLight(int a);

void defaultLight(int a)
{
    pointLight(a);
}

void spotLight(int a)
{

}

void directionalLight(int a)
{
    vec3 colour = texture(uScene, texcoord).rgb; 

    vec3 normal = normalize(texture(uNorm,texcoord).rgb);
    vec3 lightVec = LightPosition[a].xyz - texture(uPos,texcoord).xyz;
    float dist = length(lightVec);
	vec3 direc = LightDirection[a];
	vec3 reflection = reflect(-direc,normal);
	vec3 eye = normalize(- texture(uPos,texcoord).xyz);
	float viewToRe = max(dot(eye,reflection),0.0);

    float NdotL = max(dot(normal, lightVec),0.0);

   
    //The light contributes to this surface
    //Calculate attenuation (falloff)
    float attenuation = 1.0 / (Attenuation_Constant[a] + (Attenuation_Linear[a] * dist) + (Attenuation_Quadratic[a] * dist * dist));
      
    //Calculate diffuse contribution
	outColor.rgb +=  max(NdotL, 0.0) *  LightDiffuse[a] * attenuation * colour;
	//NdotL = NdotL * 0.5 + 0.5;
    //outColor.rgb += LightDiffuse[a] * attenuation * texture(uRamp, vec2(NdotL, 0.5)).rgb;
    
    //Blinn-Phong half vector
    float NdotHV =  max(dot(normal, normalize(LightDirection[a] + lightVec)), 0.0); 
        
    //Calculate specular contribution
	float spec = pow(max(viewToRe, 0.0), 16);
	outColor.rgb +=  LightSpecular[a] * spec * attenuation * colour * LightSpecularExponent[a];
    //outColor.rgb += LightSpecular[a] * texture(uRamp, vec2(viewToRe, 0.5)).rgb * attenuation;
}

void pointLight(int a)
{
	vec3 colour = texture(uScene, texcoord).rgb; 

    vec3 normal = normalize(texture(uNorm,texcoord).rgb);
    vec3 lightVec = LightPosition[a].xyz - texture(uPos,texcoord).xyz;
    float dist = length(lightVec);
	vec3 direc = lightVec / dist;
	vec3 reflection = reflect(-direc,normal);
	vec3 eye = normalize(- texture(uPos,texcoord).xyz);
	float viewToRe =  max(dot(eye,reflection),0.0);

    float NdotL = max(dot(normal, lightVec),0.0);

   
    //The light contributes to this surface
    //Calculate attenuation (falloff)
    float attenuation = 1.0 / (Attenuation_Constant[a] + (Attenuation_Linear[a] * dist) + (Attenuation_Quadratic[a] * dist * dist));
      
    //Calculate diffuse contribution
	outColor.rgb +=  max(NdotL, 0.0) *  LightDiffuse[a] * attenuation * colour;
	//NdotL = NdotL * 0.5 + 0.5;
    //outColor.rgb += LightDiffuse[a] * attenuation * texture(uRamp, vec2(NdotL, 0.5)).rgb;
    
    //Blinn-Phong half vector
    float NdotHV =  max(dot(normal, normalize(LightDirection[a] + lightVec)), 0.0); 
        
    //Calculate specular contribution
	float spec = pow(max(viewToRe, 0.0), 16);
	
    if(toonActive)
	{
		outColor.rgb += LightSpecular[a] * spec * attenuation * texture(uRamp, vec2(viewToRe, 0.5)).rgb ;
	}
	else
	{
		outColor.rgb += LightSpecular[a] * spec * colour * attenuation ;
	}
}


//float ShadowCalculation(vec4 fragPosLightSpace)
//{
//    // perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(shadowMap, projCoords.xy).r; 
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    // check whether current frag pos is in shadow
//    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
//
//    return shadow;
//}  

void main()
{

   vec3 colour = texture(uScene, texcoord).rgb; 
   
   //Ambient Light
   float ambientStrength = 1.0;
   outColor.rgb = colour * LightAmbient * ambientStrength;
    
    for(int a = 0; a < LightAmount; a++)
    { 
        if(LightEnable[a])
            switch(LightType[a])
            {        
            case POINT:
                pointLight(a);
                break;
            case DIRECTIONAL:
                directionalLight(a);
                break;
            case SPOTLIGHT:
                spotLight(a);
                break;
            default:
                defaultLight(a);
            }
    } 
    
    outColor.a = 1;
}