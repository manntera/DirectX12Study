struct in_
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

float4 main(in_ i)	:SV_TARGET
{
    return i.col;
}