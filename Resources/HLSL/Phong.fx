// ワールド x ビュー x プロジェクション 行列
float4x4 WVP;

// 回転行列
float4x4 ROT;

// 平行光源の向き
float4 LightDir;

// カメラの位置ベクトル
float4 EyePos;

// カラー
float4 Color;

// 環境光
float4 Ambient = 1.0f;

// スペキュラーの範囲
float Specular = 0.0f;

// スペキュラーの強度
float SpecularPower = 0.0f;

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
    float4 Pos	: POSITION;		// 頂点座標
	float4 Col	: COLOR0;			// 頂点カラー
	float2 Tex	: TEXCOORD0;	// テクセル座標
	float3 N		: TEXCOORD1;	// オブジェクトの法線ベクトル
	float3 L		: TEXCOORD2;	// 頂点 -> ライトの位置 ベクトル
	float3 E		: TEXCOORD3;	// 頂点 -> 視点 ベクトル
};

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS_pass (
      float4 Pos : POSITION,       // モデルの頂点
	  float4 Normal : NORMAL,	// 法線
      float2 Tex : TEXCOORD0		// テクスチャ座標
){
    VS_OUTPUT Out = (VS_OUTPUT)0;	// 出力データ
    
    // 位置座標
    Out.Pos = mul(Pos, WVP);

	Out.Tex = Tex;

	// ライト方向で入力されるので、頂点 -> ライト位置とするために逆向きに変換する。
	Out.L = normalize(-LightDir.xyz);

	// 法線ベクトル
	float4 Norm = mul(Normal, ROT);
	Out.N = Norm.xyz;

	// ライトベクトルと法線ベクトルの内積を計算し、
	// 計算結果の色の最低値を環境光(Ambient)に制限する
	Out.Col = min(max(Ambient, dot(Out.N, Out.L)), 1.0f);

	// 頂点 -> 視点 へのベクトルを計算
	Out.E = EyePos.xyz - Out.Pos.xyz;

    return Out;
}

// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS_pass(VS_OUTPUT In) : COLOR0
{   
    float4 OutColor = (float4)0;
	
	// 法線ベクトルを正規化する
	float3 N = normalize(In.N);
	// ライト位置ベクトル + 頂点 -> 視点ベクトル
	float3 H = normalize(In.L + normalize(In.E));
	// スペキュラーカラーを計算する
   float S = pow(max(0.0f, dot(N, H)), Specular) * SpecularPower;
	
	// スペキュラーカラーを加算する
	OutColor = tex2D(SrcSamp, In.Tex) * In.Col * Color + float4(S,S,S,Color.w);
	
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
