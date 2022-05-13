cbuffer perType   : register(b0) {
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

    // vector as second arg means treat as column vector
    output.position = mul(wvp, float4(input.position, 1.0f));
    output.color    = float4(input.color, 1.0f);

    return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
    return input.color;
}
