struct in_
{
    float4 pos : POSITION;
    float4 col : COLOR;
};

float4 main(in_ i)	:SV_TARGET0
{
    return i.col;
}