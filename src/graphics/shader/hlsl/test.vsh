struct vsInput {
    float2 pos : POSITION0;
    float3 tex : TEXCOORD0;
    float1 col : COLOR0;
};

struct vsOutput {
    float4 pos : SV_POSITION;
    float3 tex : TEXCOORD0;
    float1 col : COLOR0;
};

vsOutput vs_main(vsInput input) {
    vsOutput output;
    output.pos = float4(input.pos, 0.0, 1.0);
    output.tex = input.tex;
    output.col = input.col;
    return output;
}
