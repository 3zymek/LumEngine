struct VSInput {
    [[vk::location(0)]] float3 m_Position  : POSITION;
    float2 m_UV        : TEXCOORD;
};

struct VSOutput {
    float4 m_Position : SV_POSITION;
    float2 m_UV       : TEXCOORD;
};

VSOutput VSMain(VSInput input) {
    VSOutput output;

    output.m_Position = float4(input.m_Position, 1.0);
    output.m_UV = input.m_UV;

    return output;
}


struct PSInput {
    SamplerState m_FrameTexture
    float2 m_UV : TEXCOORD;
}