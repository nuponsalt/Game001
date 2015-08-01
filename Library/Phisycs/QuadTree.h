#pragma once

#include <windows.h>
#include <memory>
#include <list>
#include "TreeObject.h"

namespace KMT {

	//**線形四分木空間分割管理クラス**//
	const int CLINERQUADTREEMANAGER_LEVELMAX_ = 9;
	template <typename T> 
	class CLinerQuadTreeManager
	{
	public :
		// コンストラクタ
		CLinerQuadTreeManager(): Level_(0), Width_(0.0f), Height_(0.0f), Left_(0.0f), Top_(0.0f), unitWidth_(0.0f), unitHeight_(0.0f), CellNum_(0), Dimension_(0)
		{
			// 各レベルでの空間数を算出
			Pow_[0] = 1;
			for(int i = 1; i < CLINERQUADTREEMANAGER_LEVELMAX_ + 1; i++)
				Pow_[i] = Pow_[i - 1] * 4;
		}
		// デストラクタ
		virtual ~CLinerQuadTreeManager()
		{
			for(DWORD i = 0; i < CellNum_; i++){
				if(ppCellArray_[i] != NULL)
					delete ppCellArray_[i];
			}
			delete[] ppCellArray_;
		}
		
		// 線形四分木配列を構築する
		bool Initialize(size_t _Level, float _Left, float _Top, float _Right, float _Bottom)
		{
			//設定最高レベル以上の空間は作れない
			if(_Level >= CLINERQUADTREEMANAGER_LEVELMAX_)
				return false;
			// Levelレベル(0基点)の配列作成
			CellNum_ = (Pow_[_Level + 1] - 1) / 3;
			ppCellArray_ = new CCell<T>*[CellNum_];
			ZeroMemory(ppCellArray_, sizeof(CCell<T>*) * CellNum_);
			// 領域を登録
			Left_ = _Left;
			Top_ = _Top;
			Width_ = _Right - _Left;
			Height_ = _Bottom - _Top;
			unitWidth_ = Width_ / (1 << _Level);
			unitHeight_ = Height_ / (1 << _Level);

			Level_ = _Level;

			return true;
		}
		
		// 登録 : オブジェクト
		bool Register(float _Left, float _Top, float _Right, float _Bottom, CTreeObject<T>* _pTree)
		{
			// オブジェクトの境界範囲から登録モートン番号を算出
			DWORD elem = getMortonNumber(_Left, _Top, _Right, _Bottom);
			if(elem < CellNum_)
			{
				// 空間がない場合は新規作成
				if(!ppCellArray_[elem])
					CreateCell(elem);
				return ppCellArray_[elem]->Push(_pTree);
			}
			// 登録失敗
			return false;
		}
		
		// 衝突判定リストを作成する
		DWORD getAllCollisionList(CCollisionList<T> **_ColList)
		{
			// リスト(配列)は必ず初期化
			CollisionList_.resetPosition();
			// ルート空間の存在をチェック
			if(ppCellArray_[0] == NULL)
				// 空間が存在していない
				return 0;
			// ルート空間を処理
			std::list<T*> ColStac;
			getCollisionList(0, ColStac);

			*_ColList = &CollisionList_;

			return (DWORD)CollisionList_.getPosition();
		}

	private :
	protected :
		// おそらく次元
		size_t Dimension_;
		// 線形空間ポインタ配列
		CCell<T> **ppCellArray_;
		// べき乗数値配列
		size_t Pow_[CLINERQUADTREEMANAGER_LEVELMAX_+1];
		// 領域のXY軸幅
		float Width_, Height_;
		// 領域の左側 (X軸最小値)
		float Left_;
		// 領域の上側 (Y軸最小値)
		float Top_;
		// 最小レベル空間の幅単位
		float unitWidth_;
		// 最小レベル空間の高単位
		float unitHeight_;
		// 空間の数
		DWORD CellNum_;
		// 最下位レベル
		size_t Level_;
		// 衝突リスト
		CCollisionList<T> CollisionList_;

		// 空間内で衝突リストを作成
		bool getCollisionList(DWORD elem, std::list<T*> &_ColStac)
		{
			std::list<T*>::iterator it;
			// 1, 空間内のオブジェクト同士の衝突リスト作成
			CTreeObject<T>* pTree1 = ppCellArray_[elem]->getLatestObject();
			while(pTree1 != NULL)
			{
				CTreeObject<T>* pTree2 = pTree1->pNext;
				while(pTree2 != NULL)
				{
					CollisionList_.write(pTree1->pObject, pTree2->pObject);
					pTree2 = pTree2->pNext;
				}
				// 2, 衝突スタックとの衝突リスト作成
				for(it = _ColStac.begin(); it != _ColStac.end(); it++)
					CollisionList_.write(pTree1->pObject, *it);
				pTree1 = pTree1->pNext;
			}

			bool isChild = false;
			// 3, 子空間に移動
			DWORD ObjNum = 0;
			DWORD NextElement;
			for(DWORD i = 0; i < 4; i++)
			{
				NextElement = (elem * 4) + 1 + i;
				if(NextElement < CellNum_ && ppCellArray_[(elem * 4) + 1 + i])
				{
					if(!isChild)
					{
						// 4, 登録オブジェクトをスタックに追加
						pTree1 = ppCellArray_[elem]->getLatestObject();
						while(pTree1 != NULL)
						{
							_ColStac.push_back(pTree1->pObject);
							ObjNum++;
							pTree1 = pTree1->pNext;
						}
					}
					isChild = true;
					// 子空間へ
					getCollisionList((elem * 4) + 1 + i, _ColStac);
				}
			}
			// 5, スタックからオブジェクトを外す
			if(isChild)
				for(DWORD i = 0; i < ObjNum; i++)
					_ColStac.pop_back();
			return true;
		}
		
		// 空間を生成
		bool CreateCell(DWORD elem)
		{
			// 引数の要素番号
			while(!ppCellArray_[elem])
			{
				// 指定の要素番号に空間を新規作成
				ppCellArray_[elem] = new CCell<T>;
				// 親空間にジャンプ
				elem = (elem - 1) >> 2;
				if(elem >= CellNum_)
					break ;
			}
			return true;
		}
		
		// 座標から空間番号を算出
		DWORD getMortonNumber(float _Left, float _Top, float _Right, float _Bottom)
		{
			// 最小レベルにおける各軸位置を算出
			DWORD LT = getPointElement(_Left, _Top);
			DWORD RB = getPointElement(_Right, _Bottom);
			// 空間番号の排他的論理和から所属レベルを算出
			DWORD Def = RB ^ LT;
			size_t HiLevel = 0;
			for(size_t i = 0; i < Level_; i++)
			{
				DWORD Check = (Def >> (i * 2)) & 0x3;
				if(Check != 0)
					HiLevel = i + 1;
			}
			DWORD SpaceNum = RB >> (HiLevel * 2);
			DWORD AddNum = (Pow_[Level_ - HiLevel] - 1) / 3;
			SpaceNum += AddNum;
			if(SpaceNum > CellNum_)
				return 0xffffffff;
			return SpaceNum;
		}
		
		// ビット分割
		DWORD BitSeparate32(DWORD n)
		{
			n = (n | (n << 8)) & 0x00ff00ff;
			n = (n | (n << 4)) & 0x0f0f0f0f;
			n = (n | (n << 2)) & 0x33333333;
			return (n | (n << 1)) & 0x55555555;
		}
		
		// 2Dモートン空間番号算出関数
		WORD get2DMortonNumber(WORD x, WORD y) { return (WORD)(BitSeparate32(x) | (BitSeparate32(y) << 1)); }
		
		// 座標→線形四分木要素番号変換関数
		inline DWORD getPointElement(float posX, float posY) { return get2DMortonNumber((WORD)((posX - Left_) / unitWidth_), (WORD)((posY - Top_) / unitHeight_)); }

	};

}