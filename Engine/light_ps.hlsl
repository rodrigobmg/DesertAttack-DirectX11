////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D grass : register(t0);
Texture2D dirt : register(t1);
Texture2D blend : register(t2);

SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
	float padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 grassColor;
    float4 dirtColor;
    float4 blendColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;


	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    grassColor = grass.Sample(SampleType, input.tex);
    dirtColor = dirt.Sample(SampleType, input.tex);
    blendColor = blend.Sample(SampleType, input.tex);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    color = saturate(diffuseColor * lightIntensity);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * lerp(grassColor, dirtColor, blendColor.x);

    return color;
}