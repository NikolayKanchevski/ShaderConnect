static const float2 _19[3] = { (-0.5f).xx, float2(0.5f, -0.5f), float2(0.0f, 0.5f) };
static const float3 _28[3] = { float3(1.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.0f, 0.0f, 1.0f) };

static float4 gl_Position;
static int gl_VertexIndex;
static float3 _49;

struct SPIRV_Cross_Input
{
    uint gl_VertexIndex : SV_VertexID;
};

struct SPIRV_Cross_Output
{
    float3 _49 : TEXCOORD0;
    float4 gl_Position : SV_Position;
};

void main_inner()
{
    gl_Position = float4(_19[gl_VertexIndex], 0.0f, 1.0f);
    _49 = _28[gl_VertexIndex];
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    gl_VertexIndex = int(stage_input.gl_VertexIndex);
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output._49 = _49;
    return stage_output;
}
