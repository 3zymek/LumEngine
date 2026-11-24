struct VSInput {
    [[vk::location(0)]] float3 m_Position : POSITION;
    [[vk::location(1)]] float2 m_UV : TEXCOORD;
};

struct VSOutput {
    float4 m_Position : SV_POSITION;
    [[vk::location(0)]] float2 m_UV : TEXCOORD;
};

VSOutput VSMain(VSInput input) {
    VSOutput output;

    output.m_Position = float4(input.m_Position, 1.0);
    output.m_UV = input.m_UV;

    return output;
}





[[vk::binding(0, 0)]] Texture2D<float4> in_FrameTexture : register(t0, space0);
[[vk::binding(1, 0)]] SamplerState in_FrameSampler : register(s0, space0);

struct PSInput {
    [[vk::location(0)]] float2 m_UV : TEXCOORD;
}

struct PSOutput {
    float4 m_Color : COLOR;
}

PSOutput PSMain(PSInput input) {

    VSOutput output;
    output.m_Color = input.m_FrameTexture.Sample(input.m_FrameSampler, input.m_UV);

}