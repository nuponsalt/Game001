//
//  wsb_xml.h
//
//  Created by 常木 徹 on 12/02/05.
//  Copyright (c) 2011 t.tsuneki All rights reserved.
//


#ifndef __WSB_XML__
#define __WSB_XML__

#include <iostream>
#include <string>

#include <libxml/xmlversion.h>
#include <libxml/xmlreader.h>
#include <libxml/tree.h>
#include <memory>

using namespace std ;
class CWsbXml ;
typedef std::shared_ptr< CWsbXml > CWsbXmlSP ;
typedef std::weak_ptr< CWsbXml > CWsbXmlWP ;

//-------------------------------------------------------------------------
//
// XML クラス
//
class CWsbXml {
	
private:

	// ユーザによるコンストラクタ呼び出し禁止
	CWsbXml() {
	/*
		m_tag_child = NULL ;
		m_tag_next = NULL ;
		m_tag_parent = NULL ;
		m_attr = NULL ;
	*/
	}
	
	std::string		m_file_name ;	// ファイル名
	std::string		m_tag_name ;	// タグ名
	std::string		m_string ;		// 要素の文字列
	int				m_s32 ;			// 要素の整数
	float			m_f32 ;			// 要素の少数
	
	CWsbXmlSP		m_tag_child ;
	CWsbXmlSP		m_tag_next ;
	CWsbXmlWP		m_this_week ;
	CWsbXmlWP		m_tag_parent ;
	CWsbXmlSP		m_attr ;
	
	static int ParseProcess( xmlTextReaderPtr _reader, CWsbXmlSP _parent, int _prevtype ) ;
	
public:
	
	//=====================================================
	// 要素情報の取得
	//=====================================================
    inline std::string GetFileName(){ return GetElementTop()->m_file_name ; }
	inline std::string GetTagName(){ return m_tag_name ; }
	inline std::string GetString(){ return m_string ; }
	inline int GetInt(){ return m_s32 ; }
	inline float GetFloat(){ return m_f32 ; }
	inline CWsbXmlSP GetParentElement(){ return CWsbXmlSP( m_tag_parent ) ; }
	
	//=====================================================
	// 要素の取得
	//=====================================================
	int	GetElementNum( std::string _tag_name = "all_element" ) ;
	CWsbXmlSP GetElement( std::string _tag_name, int _idx = 0 ) ;
	CWsbXmlSP GetElement( int _idx ) ;
	CWsbXmlSP GetElementTop() ;
	
	//=====================================================
	// 属性の取得
	//=====================================================
	int GetAttrNum( std::string _attr_name = "all_attribute" ) ;
	CWsbXmlSP GetAttr( std::string _attr_name, int _idx = 0 ) ;
	CWsbXmlSP GetAttr( int _idx ) ;

	//=====================================================
	// テキストベースの XMLを取得
	// arg1... 出力結果
	// arg2... 指定しなくてOK
	//=====================================================
	void GetXmlString( std::string *_string, bool start = true ) ;
	
	//=====================================================
	// 要素の設定
	//=====================================================
	void Set( int _v ) ;
	void Set( float _v ) ;
	void Set( std::string _string ) ;
	
	//=====================================================
	// 子供の追加
	// arg1... タグ名
	// arg2... string, s32, f32 のどれか
	//=====================================================
	template<class T> void AddChild( string _string, const T& _v )
	{
		CWsbXmlSP sarch = CWsbXmlSP( this->m_this_week ) ;
		
		if( sarch->m_tag_child == NULL ){
			sarch->m_tag_child = CWsbXmlSP( new CWsbXml ) ;
			sarch = sarch->m_tag_child ;
		}else{
			sarch = sarch->m_tag_child ;
			while( sarch->m_tag_next != NULL ){
				sarch = sarch->m_tag_next ;
			}
			sarch->m_tag_next = CWsbXmlSP( new CWsbXml ) ;
			sarch = sarch->m_tag_next ;
		}
		
		sarch->m_tag_parent = this->m_this_week ;
		sarch->m_this_week = CWsbXmlWP( sarch ) ;
		sarch->Set( _v ) ;
		sarch->m_tag_name = _string ;
	}
	
	//=====================================================
	// 兄弟の追加
	// arg1... タグ名
	// arg2... string, s32, f32 のどれか
	//=====================================================
	template<class T> bool AddNext( string _string, const T& _v )
	{
		if( this->m_string == "wsb_root_xml" ) return false ;
		
		CWsbXmlSP sarch = CWsbXmlSP( this->m_this_week ) ;
		while( sarch->m_tag_next != NULL ){
			sarch = sarch->m_tag_next ;
		}
		sarch->m_tag_next = CWsbXmlSP( new CWsbXml ) ;
		sarch->m_tag_parent = this->m_tag_parent ;
		sarch = sarch->m_tag_next ;
		sarch->m_this_week = CWsbXmlWP( sarch ) ;
		sarch->Set( _v ) ;
		sarch->m_tag_name = _string ;
		return true ;
	}
	
	
public:
	
	static CWsbXmlSP CreateXml( std::string _root_name ) ;
	static CWsbXmlSP LoadXmlFile( std::string _filename ) ;
	static CWsbXmlSP LoadXmlString( std::string _xmlstring ) ;
	void SaveXmlFile( std::string _filename ) ;

	//=====================================================
	// 自身のコピー
	//=====================================================
	CWsbXml GetInstance() { return (*this) ; }	
	
	//=====================================================
	// 要素の取得
	//=====================================================
	CWsbXml operator[]( std::string index ) {
		CWsbXmlSP wkxml = this->GetElement( index ) ;
		return (*wkxml.get()) ;
	}
	
	CWsbXml operator[]( int index ) {
		CWsbXmlSP wkparent = this->GetParentElement() ;
		CWsbXmlSP wkxml = wkparent->GetElement( this->m_tag_name, index ) ;
		return (*wkxml.get()) ;
	}
	
	//=====================================================
	// 属性の取得
	//=====================================================
	CWsbXml operator()( std::string index ) {
		CWsbXmlSP wkxml = this->GetAttr( index ) ;
		return (*wkxml.get()) ;
	}
	
	//=====================================================
	// 文字列データの取得
	//=====================================================
	std::string operator()() {
		return this->GetString() ;
	}
	
} ;

#endif
