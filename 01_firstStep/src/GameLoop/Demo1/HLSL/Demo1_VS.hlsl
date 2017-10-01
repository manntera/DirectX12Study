struct in_
{
    float4 pos : POSITION;
    float4 col : COLOR;
};

struct out_
{
    float4 pos : POSITION;
    float4 col : COLOR;
};

out_ main(in_ i)
{
    out_ o;

    o.pos = i.pos;
    o.col = i.col;

    return o;
}
