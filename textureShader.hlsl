cbuffer ConstantBuffer
{
    matrix worldViewProjection;
    matrix world;
    float4 materialColour;
    float4 ambientLightColour;
    float4 directionalLightColour;
    float4 directionalLightVector;
    
    float3 eyePosition;
    float specularPower;
    //float3 Pad1;
    //float Pad2;
};

Texture2D Texture;
SamplerState ss;

struct VertexIn
{
    float3 InputPosition : POSITION;
    float2 TexCoord      : TEXCOORD;
    float3 Normal        : NORMAL;
};

struct VertexOut
{
    float4 OutputPosition : SV_POSITION;
    float2 TexCoord       : TEXCOORD;
    float4 Normal         : TEXCOORD1;
    float4 WorldPosition  : TEXCOORD2;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to homogeneous clip space.
    vout.OutputPosition = mul(worldViewProjection, float4(vin.InputPosition, 1.0f));
	
	// transform normal to world space
    float4 adjustedNormal = mul(world, float4(vin.Normal, 0.0f));
    
    vout.TexCoord = vin.TexCoord;
    
    // Populate Normal and WorldPosition in output
    vout.Normal = adjustedNormal;
    vout.WorldPosition = mul(world, float4(vin.InputPosition, 1.0f));    
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float shininess = 32.0; // higher number more shiny
    
    // Calculate diffuse lighting
    float diffuseAmount = saturate(dot(pin.Normal, -directionalLightVector));
    float4 diffuseColour = materialColour * (diffuseAmount * directionalLightColour + ambientLightColour);
    float4 textureColour = diffuseColour * Texture.Sample(ss, pin.TexCoord);
    
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

    float4 finalColour = textureColour + specular;
    
    //return finalColour * Texture.Sample(ss, pin.TexCoord);
    //return materialColour;
    //return textureColour;
    //return directionalLightColour;
    //return ambientLightColour;
    return finalColour;
    //return diffuseColour;

}
