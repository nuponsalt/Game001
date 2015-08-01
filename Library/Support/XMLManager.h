#pragma once

#include "wsb_xml.h"

class CXMLManager
{
public :
	// ランバートシェーダー
	static CWsbXmlSP LambertShadingParameter;
	// エフェクトがけなし

	// 初期化
	static void Initialize();
} ;