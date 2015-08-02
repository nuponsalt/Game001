// カラー
float4 Color;

// ワールド x ビュー x プロジェクション 行列
float4x4 WVP;

// -------------------------------------------------------------
// テクスチャ
// -------------------------------------------------------------
texture SrcTex;
sampler SrcSamp = sampler_state
{
    Texture = <SrcTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;
	float  Size			: PSIZE;
	float2 Tex			: TEXCOORD0;
};

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS_pass1 (
      float4 Pos    : POSITION,         // モデルの頂点
	  float Size	: PSIZE,			// 頂点サイズ
      float4 Tex    : TEXCOORD0	        // テクスチャ座標
){

    VS_OUTPUT Out = (VS_OUTPUT)0;       // 出力データ
    
    // 位置座標
    Out.Pos = mul(Pos, WVP);

	// テクスチャ
    Out.Tex = Tex ;

	// ポイントサイズ
	Out.Size = Size ;

    return Out;
}

// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS_pass1(VS_OUTPUT In) : COLOR
{   
    float4 OutColor;
	OutColor = tex2D(SrcSamp, In.Tex) * Color;
    return OutColor;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader
{

	// 通常合成
    pass Normal
    {
		AlphaBlendEnable = true ;
		SrcBlend = SRCALPHA ;
		DestBlend = INVSRCALPHA ;

        // シェーダ
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }

	// 加算合成
    pass Add
    {
		AlphaBlendEnable = true ;
		SrcBlend = ONE ;
		DestBlend = ONE ;

        // シェーダ
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }

}
