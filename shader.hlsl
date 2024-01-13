cbuffer ConstantBuffer
{
    matrix worldViewProjection;
    matrix world;
    float4 materialColour;
    float4 ambientLightColour;
    float4 directionalLightColour;
    float4 directionalLightVector;
    float3 eyePosition; // Added eye position
    float  specularPower; // Added specular power
    
    
    //float3 pad1; // Padding to ensure the constant buffer size is a multiple of 16
    //float pad2;
};

struct VertexIn
{
    float3 InputPosition : POSITION;
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 OutputPosition : SV_POSITION;
    float4 Normal : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
    // Transform to homogeneous clip space.
    vout.OutputPosition = mul(worldViewProjection, float4(vin.InputPosition, 1.0f));

    // Transform normal to world space
    float4 adjustedNormal = mul(world, float4(vin.Normal, 0.0f));

    // Populate Normal and WorldPosition in output
    vout.Normal = adjustedNormal;
    vout.WorldPosition = mul(world, float4(vin.InputPosition, 1.0f));
    //vout.LightPosition = mul(lightViewProjection, float4(vin.InputPosition, 1.0f));
    
    return vout;
	
}

float4 PS(VertexOut pin) : SV_Target
{    
    float shininess = 32.0; // higher number more shiny
    
    // Calculate diffuse lighting
    float diffuseAmount = saturate(dot(pin.Normal, -directionalLightVector));
    float4 diffuseColor = materialColour * (diffuseAmount * directionalLightColour + ambientLightColour);
    
    // Normalize the normal vector
    float3 normal = normalize(pin.Normal.xyz);

    // Calculate the light direction
    float3 lightDir = normalize(-directionalLightVector.xyz);

    // Calculate the view direction
    float3 viewDir = normalize(eyePosition - pin.WorldPosition.xyz);

    // Calculate the reflection vector
    float3 reflection = reflect(-lightDir, normal);

    // Calculate the specular intensity using the Phong reflection model
    float specularIntensity = pow(saturate(dot(reflection, viewDir)), shininess);

    // Combine diffuse, and specular lighting
    float4 specular = directionalLightColour * specularIntensity;

    float4 finalColor = diffuseColor + specular;
    
    return finalColor;
    
    //float3 normal = normalize(pin.Normal.xyz);
    //float specularPower = 60.0f;
    //pad1 = 1.0f;
    //pad2 = 1.0f;

    // Calculate diffuse lighting
    //float diffuseAmount = saturate(dot(pin.Normal, -directionalLightVector));
    //float4 diffuseColor = materialColour * (diffuseAmount * directionalLightColour + ambientLightColour);
    
    

    // Calculate specular highlights (using Blinn-Phong)
    //float3 lightDir = normalize(-directionalLightVector.xyz);
    //float3 halfwayDir = normalize(lightDir + toEye);
    //float specular = pow(saturate(dot(pin.Normal.xyz, halfwayDir)), specularPower);
    //float4 specularColor = specular * directionalLightColour;
    
    //float3 lightSource = (-directionalLightVector.xyz);
    //float dotNL = (2 * (dot(pin.Normal.xyz, -directionalLightColour.xyz)));
    //float phongSpecular = saturate(dotNL * pin.Normal.xyz - lightSource);
    //float4 specularColor = phongSpecular * directionalLightColour;

    //return diffuseColor+specularColor;
    
    //return float4(finalColor, 1.0);
    
}