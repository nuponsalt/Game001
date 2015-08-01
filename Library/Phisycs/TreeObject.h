#pragma once
#include <memory>

namespace KMT
{
	// 前方宣言
	template<typename T> 
	class CCell;

	//**分木登録オブジェクト**//
	template<typename T>
	class CTreeObject
	{
	public:
		// 登録空間
		CCell<T> *pCell;
		// 判定対象オブジェクト
		T *pObject;
		// 前のTreeObjectポインタ
		CTreeObject<T> *pPrevious;
		// 次のTreeObjectポインタ
		CTreeObject<T> *pNext;
		// 
		int ID;

		// コンストラクタ
		CTreeObject(int _id = 0) : ID(_id), pCell(NULL), pObject(NULL), pPrevious(NULL), pNext(NULL) { }
		// デストラクタ
		virtual ~CTreeObject() { }

		// 自らリストから外れる
		bool Remove()
		{
			// すでに逸脱している時は処理終了
			if(!pCell)
				return false;
			// 自分を登録している空間に自身を通知
			if(!pCell->OnRemove(this))
				return false;
			// 逸脱処理
			// 前後のオブジェクトを結びつける
			if(pPrevious != NULL)
				pPrevious->pNext = pNext;
			if(pNext != NULL)
				pNext->pPrevious = pPrevious;
			pPrevious = NULL;
			pNext = NULL;
			pCell = NULL;
			return true;
		}

	};

	//**衝突リスト**//
	template <typename T>
	class CCollisionList
	{
	public :
		// コンストラクタ
		CCollisionList() : ppRoot_(0), position_(0), mallocSize_(0) 
		{ 
			ppRoot_ = (T**)malloc(0); 
		}
		// デストラクタ
		~CCollisionList() { }

		// 取得 : 
		inline size_t getPosition() { return position_; }
		// 取得 : ルートポインタ
		inline T** getRootPtr() { return ppRoot_; }
		// 再設定 : 位置
		inline void resetPosition() { position_ = 0; }
		
		// 
		void write(T *a, T *b)
		{
			const int COLLISIONLIST_REALLOCSIZE = 100;
			if(position_ >= mallocSize_)
			{
				ppRoot_ = (T**)realloc(ppRoot_, sizeof(T*) * (mallocSize_ + COLLISIONLIST_REALLOCSIZE));
				mallocSize_ += COLLISIONLIST_REALLOCSIZE;
			}
			ppRoot_[position_++] = a;
			ppRoot_[position_++] = b;
		}
		// 
		void refresh() { if(ppRoot_) free(ppRoot_); }

	private :
		// リストルートポインタ
		T** ppRoot_;
		// 配列位置
		size_t	 position_;
		size_t	 mallocSize_;

	};
	
	//**空間**//
	template <typename T>
	class CCell
	{
	public :
		// コンストラクタ
		CCell() : pLatest(NULL) {}
		// デストラクタ
		virtual ~CCell() { if(pLatest != NULL) ResetLink(pLatest); }
		
		// リンクを全てリセットする
		void ResetLink(CTreeObject<T>* pTree)
		{
			if(pTree->pNext != NULL)
				ResetLink(pTree->pNext);
		}
		
		// ツリーオブジェクトを追加
		bool Push(CTreeObject<T> *pTree)
		{
			// 無効オブジェクトは登録しない
			if(pTree == NULL) return false;
			// 2重登録チェック
			if(pTree->pCell == this) return false;
			if(pLatest == NULL)
				pLatest = pTree;
			else
			{
				// 最新TreeObjectを更新
				pTree->pNext = pLatest;
				pLatest->pPrevious = pTree;
				pLatest = pTree;
			}
			// 空間を登録
			pTree->pCell = this;
			return true;
		}
		
		// 取得 : 最新ツリーオブジェクト
		inline CTreeObject<T>* getLatestObject() { return pLatest; }
		
		// 削除されるオブジェクトをチェック
		bool OnRemove(CTreeObject<T>* pRemoveObj)
		{
			if(pLatest == pRemoveObj)
			{
				// 次のオブジェクトに挿げ替え
				if(pLatest != NULL)
					pLatest = pLatest->pNext;
			}
			return true;
		}

	protected :
		// 最新ツリーオブジェクトのポインタ
		CTreeObject<T> *pLatest;

	};
}