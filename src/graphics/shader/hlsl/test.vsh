struct vsInput {
    float4 pos : POSITION0;
    float2 tex : TEXCOORD0;
};

struct vsOutput {
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

vsOutput vs_main(vsInput input) {
    vsOutput output;
    output.pos = input.pos;
    output.tex = input.tex;
    return output;
}
