//*********************************************************************
// クォータニオンクラス
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#pragma once

namespace KMT {
	
	// クォータニオン
	class CQuaternion : public D3DXQUATERNION {
	public :

		// コンストラクタ
		// 単位クォータニオンを生成
		CQuaternion() : D3DXQUATERNION( 0, 0, 0, 1 ){}

		// 指定した要素で生成
		CQuaternion( float x, float y, float z, float w ) : D3DXQUATERNION( x, y, z, w ){}

		// 指定した軸と角度の回転を表すクォータニオンを生成
		// 指定角度はラジアン値
		CQuaternion( const CVector3& axis, float angle ) ;

		// 指定した軸と角度の回転を表すクォータニオンを生成
		// 上記コンストラクタと機能は同等
		friend CQuaternion RotationAroundAxis( const CVector3& axis, float angle ){
			return CQuaternion( axis, angle ) ;
		}
		
		// 指定軸の回転を表すクォータニオンを生成
		// 軸の長さが角度になる
		CQuaternion( const CVector3& axis ) ;

		// 指定ベクトル間の回転を表すクォータニオンを生成
		// ベクトルは正規化しておく必要ない
		CQuaternion( CVector3 vfrom, CVector3 vto ) ;

		// 指定ベクトル間の回転を表すクォータニオンを生成
		// ベクトルは正規化しておく必要ない
		// 上記コンストラクタと機能は同等
		friend CQuaternion RotationBetweenVectors(const CVector3& vfrom, const CVector3& vto) {
			return CQuaternion( vfrom, vto ) ;
		} 

		// 指定ベクトル間の回転を表すクォータニオンを生成
		// ベクトルは正規化しておく必要ない
		// 指定した軸を中心に回転するので、カメラなどで天地を反転させたくない場合などに使用
		CQuaternion( CVector3 vfrom, CVector3 vto, CVector3 axis ) ;

		// 指定ベクトル間の回転を表すクォータニオンを生成
		// ベクトルは正規化しておく必要ない
		// 指定した軸を中心に回転するので、カメラなどで天地を反転させたくない場合などに使用
		// 機能は該当するコンストラクタと同等
		friend CQuaternion RotationBetweenVectorsAroundAxis( const CVector3& vfrom, const CVector3& vto, const CVector3& axis ) {
			return CQuaternion( vfrom, vto, axis );
		}

		// 球面線形補間
		// f = 0のときv1, f = 1のときv2
		CQuaternion( const CQuaternion& v1, const CQuaternion& v2, float f ) ;

		// D3DXQUATERNIONからの変換
		CQuaternion( const D3DXQUATERNION& q ) ;

		// D3DXQUATERNIONへの変換
		operator D3DXQUATERNION() const ;

		// 代入
		CQuaternion& operator= ( const CQuaternion& q ) ;

		// 正規化
		// 既存のクォータニオンに代入
		CQuaternion& Normalize() ;

		// 正規化
		// 結果のクォータニオンを生成
		friend CQuaternion Normalize( CQuaternion q ) {
			return q.Normalize() ;
		}

		// 逆クォータニオン
		// 既存のクォータニオンに代入
		CQuaternion& Inverse() ;
		
		// 逆クォータニオン
		// 結果のクォータニオンを生成
		friend CQuaternion Inverse( CQuaternion q ) {
			return q.Inverse() ;
		}

		CQuaternion& Delta( CQuaternion q ) ;

		friend CQuaternion Delta( CQuaternion q1, CQuaternion q2 ) {
			return q1.Delta( q2 ) ;
		}

	} ;

}