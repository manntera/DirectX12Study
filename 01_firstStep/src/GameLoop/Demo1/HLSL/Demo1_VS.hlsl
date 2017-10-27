struct in_
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct out_
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

out_ main(in_ i)
{
    out_ o;

    o.pos = float4(i.pos, 1);
    o.col = i.col;

    return o;
}
