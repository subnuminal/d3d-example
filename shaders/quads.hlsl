cbuffer perType   : register(b0) {
    // column major by default, so:
    // { 1, 2, 3, 4,
    //   5, 6, 7, 8,
    //   ...
    // }
    // in code is:
    // | 1 5 ... |
    // | 2 6 ... |
    // | 3 7 ... |
    // | 4 8 ... |
    matrix wvp;
}

struct vs_in {
    // vector values are row major by default
    float3 position : POS;
    float3 normal   : NOR;
    float2 texcoord : TEX;
    float3 color    : COL;
};

struct vs_out {
    float4 position : SV_POSITION;
    float4 color    : COL;
};

vs_out vs_main(vs_in input) {
    vs_out output;

    output.position = mul(wvp, float4(input.position, 1.0f));
    output.color    = float4(input.color, 1.0f);

    return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
    return input.color;
}
