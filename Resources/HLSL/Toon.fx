// ワールド x ビュー x プロジェクション 行列
float4x4 WVP;

// 回転行列
float4x4 ROT;

// 平行光源の方向ベクトル
float4 LightDir;

// マテリアルのカラー
float4 Color;

// ---------------------------------------------------
// テクスチャー
// ---------------------------------------------------
// オブジェクトのテクスチャー
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

// トゥーンマップテクスチャー
texture ToonTex;
sampler ToonSamp = sampler_state
{
	Texture = <ToonTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};

// ----------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// ----------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos		:	POSITION;
	float2 Tex		:	TEXCOORD0;
	float3 Normal	:	TEXCOORD1;
};

VS_OUTPUT VS_pass(	float4 Pos		:	POSITION,
								float4 Normal	:	NORMAL,
								float2 Tex		:	TEXCOORD0 )
{
	VS_OUTPUT Out;

	Out.Pos = mul(Pos, WVP);
	Out.Tex = Tex;

	float4 N = mul(Normal, ROT);
	Out.Normal = N.xyz;

	return Out;
}

float4 PS_pass(VS_OUTPUT In) : COLOR0
{
	float4 OutColor;

	// ハーフランバート拡散照明によるライティング計算
	float p = dot(In.Normal, normalize(-LightDir));
	p = p * 0.5f + 0.5f;
	p = p * p;

	// トゥーンシェーダー処理
	// 色情報をテクセルのU成分とし、
	// トゥーンマップテクスチャから光の反射率を取得
	float4 Col = tex2D(ToonSamp, float2(p, 0.0f));

	// 色情報を格納する
	OutColor = tex2D(SrcSamp, In.Tex) * Col * Color;

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
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

        // シェーダ
        VertexShader = compile vs_1_1 VS_pass();
        PixelShader  = compile ps_2_0 PS_pass();
		Sampler[0] = (SrcSamp);
    }

	// 加算合成
    pass Add
    {
		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = ONE;

        // シェーダ
        VertexShader = compile vs_1_1 VS_pass();
        PixelShader  = compile ps_2_0 PS_pass();
		Sampler[0] = (SrcSamp);
    }
}