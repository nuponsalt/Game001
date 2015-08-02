// ワールド x ビュー x プロジェクション 行列
float4x4 WVP;

// 回転行列
float4x4 ROT;

// 平行光源の向き
float4 LightDir;

// 視点
float4 EyePos;

// カラー
float4 Color;

// 環境光に対するマテリアル
// 0.0f～1.0fの値を持ち、1.0fのとき最も強くなる
float4 Ambient = 0.0f;

// フォグカラー
float4 fogColor;
// フォグの開始位置
float4 fogCoord = float4(0, 1, 0, 0);

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
    float4 Pos : POSITION;
	float4 Col : COLOR0;
	float2 Tex : TEXCOORD0;
	float4 LocalPos : TEXCOORD1;
};

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS_pass1 (
		float4 Pos : POSITION,	// モデルの頂点
		float4 Normal : NORMAL,	// 法線
		float2 Tex : TEXCOORD0	// テクスチャ座標
){
    VS_OUTPUT Out = (VS_OUTPUT)0;	// 出力データ
    
    // 位置座標
    Out.Pos = mul(Pos, WVP);

	// 照明を逆方向にする
	float3 L = normalize(-LightDir.xyz);

	// 法線ベクトル
	float4 Norm = mul(Normal, ROT);
	float3 N = normalize(Norm.xyz);

	// 照明と法線から内積を計算する
	// またmax関数により頂点カラーの最小値を環境光に抑えるように設定する
	Out.Col = max(Ambient, dot(N, L));

	Out.Tex = Tex;

	Out.LocalPos = Out.Pos;

    return Out;
}

// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS_pass1(VS_OUTPUT In) : COLOR0
{   
    float4 OutColor = (float4)0;

	// 頂点と視点との距離を計算する
	float D = distance(In.LocalPos.xyz, EyePos.xyz);
	//
	float F = pow(1.0f - pow(In.LocalPos.z / In.LocalPos.w, fogCoord.x), fogCoord.y);

	// オブジェクトのランバート拡散照明の計算結果とフォグカラーを線形合成する
	OutColor = (tex2D(SrcSamp, In.Tex) * In.Col * F + (fogColor * (1.0f - F))) * Color;

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
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }

	// 加算合成
    pass Add
    {
		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = ONE;

        // シェーダ
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }


}
