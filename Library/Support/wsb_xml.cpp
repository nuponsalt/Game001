#include "DXUT.h"
#include "wsb_xml.h"
#include <stdlib.h>
#include <fstream>

#define WSB_XML_FAIL				(-1)
#define WSB_XML_FALSE				(0)
#define WSB_XML_TRUE				(1)
#define XML_READER_TYPE_ROOT		(99)
#define WSB_XML_PRINT_DEBUG			(0)

using namespace std ;

#pragma warning( push )
#pragma warning( disable: 4127 )



void Print( const xmlChar *str ) {
//	std::cout << (const char*)str << std::endl ;
}

void Print( std::string str ) {
//	std::cout << str << std::endl ;
}


//==================================================================
//	name....LoadXMLFile
//	work....ファイルからの読み込み
//	arg1....ファイル名
//	ret.....[ 成功 : CWsbXml へのポインタ ]  [ 失敗 : NULL ]
//	tips....none
//==================================================================
CWsbXmlSP CWsbXml::LoadXmlFile( std::string _filename ) {

	// ファイルパスの取得
	xmlTextReaderPtr wkreader ;
	
	// XML リーダーの読み込み
	wkreader = xmlNewTextReaderFilename( _filename.c_str() ) ;
	
	CWsbXmlSP wkxml ;
	CWsbXmlSP wknullobj ;
	if ( NULL == wkreader ) {
		xmlFreeTextReader( wkreader ) ;
		return wknullobj ;
	}
	
	// パース開始
	int wkresult = xmlTextReaderRead( wkreader ) ;
	if ( WSB_XML_TRUE == wkresult ) {
		wkxml = CWsbXmlSP( new CWsbXml ) ;
		wkxml->m_this_week = CWsbXmlWP( wkxml ) ;
		wkresult = CWsbXml::ParseProcess( wkreader, wkxml, XML_READER_TYPE_ROOT ) ;
	}
	xmlFreeTextReader( wkreader ) ;
	
	// 解析失敗
	if ( wkresult == WSB_XML_FAIL ){
		return wknullobj ;
	}
	
    wkxml->m_file_name = _filename ;
	return wkxml ;
}


//==================================================================
//	name....LoadXmlString
//	work....文字列データからXML を作成
//	arg1....文字列データ
//	ret.....[ 成功 : CWsbXml へのポインタ ]  [ 失敗 : NULL ]
//	tips....none
//==================================================================
CWsbXmlSP CWsbXml::LoadXmlString( std::string _xmlstring ) {
	
    xmlParserInputBufferPtr wkinputbuf = xmlAllocParserInputBuffer(XML_CHAR_ENCODING_UTF8);
     
	CWsbXmlSP wkxml ;
	CWsbXmlSP wknullobj ;
    
	if( !wkinputbuf ) return wknullobj ;
	
	xmlBufferPtr wkxmlbuf = xmlBufferCreateStatic( (void*)_xmlstring.c_str(), _xmlstring.length() ) ;
	wkinputbuf->buffer = wkxmlbuf ;
	
	xmlTextReaderPtr wkreader = xmlNewTextReader( wkinputbuf, NULL ) ;
	if ( NULL == wkreader ) {
		xmlFreeTextReader( wkreader ) ;
		return wknullobj ;
	}

	/*
	 *	解析処理
	 */
	int wkresult = xmlTextReaderRead( wkreader ) ;
	if ( WSB_XML_TRUE == wkresult ) {
		wkxml = CWsbXmlSP( new CWsbXml ) ;
		wkxml->m_this_week = CWsbXmlWP( wkxml ) ;
		wkresult = CWsbXml::ParseProcess( wkreader, wkxml, XML_READER_TYPE_ROOT ) ;
	}
	xmlFreeTextReader( wkreader ) ;
	
	// 解析失敗
	if ( wkresult == WSB_XML_FAIL ){
		return wknullobj ;
	}
	
	return wkxml ;
}

//=======================================================================================
//	name... ParseProcess
//	work... パース処理
//	arg1... xmlTextReaderPtr
//	ret.... [ 失敗 -1 ]	[ なし 0 ] [ 成功 1 ] 
//	tips	:	none
//=======================================================================================
int wknowtype, wknexttype ;
const xmlChar *wkname, *wkvalue ;
CWsbXmlSP wkxml ;
CWsbXmlSP wk_add ;
CWsbXmlSP wk_attr_xml ;
int CWsbXml::ParseProcess( xmlTextReaderPtr _reader, CWsbXmlSP _prevxml, int _prevtype ) {
	
	
	// xmlTextReaderPtr が存在しない
	if ( _reader == NULL ) {
		#if WSB_XML_PRINT_DEBUG
		Print( "err : _reader is NULL" ) ;
		#endif
		return WSB_XML_FAIL ;
	}
	
	// 要素タイプの取得
	if ( ( wknowtype = xmlTextReaderNodeType( _reader ) ) == WSB_XML_FAIL ) {
		#if WSB_XML_PRINT_DEBUG
		Print( "err : 要素タイプ is -1" ) ;
		#endif
		return WSB_XML_FAIL ;
	}
	
	/*
	 *
	 *	要素タイプによって挙動が変化
	 *
	 */
	switch ( wknowtype ) {
			
		/*
		 *	開始要素
		 */
		case XML_READER_TYPE_ELEMENT :
			{
				// 要素名の取得
				if ( ( wkname = xmlTextReaderConstName( _reader ) ) == NULL ) {
					#if WSB_XML_PRINT_DEBUG
					Print( "err : 要素名が存在しない" ) ;
					#endif
					return WSB_XML_FAIL ;
				}
				#if WSB_XML_PRINT_DEBUG
				Print( wkname ) ;
				#endif
				
				/*
				 *	要素が空かどうかを判定
				 *	属性を取得した後だとちゃんととれなかったのでこの時点で
				 *	空要素かどうかの判定を取る
				 */
				int wkisempty = xmlTextReaderIsEmptyElement( _reader ) ;

				/*
				 *	前回のタイプによって挙動が変化
				 */
				switch ( _prevtype ) {
						
					/*
					 *	前回がタグの開始であれば、child を生成
					 *	_prevxml は parent
					 */
					case XML_READER_TYPE_ELEMENT :
						{
							wkxml = CWsbXmlSP( new CWsbXml ) ;
							wkxml->m_this_week = CWsbXmlWP( wkxml ) ;
//							wkxml->m_tag_name = toString( wkname ) ;
							wkxml->m_tag_name = string( (const char*)wkname );

							wkxml->m_tag_parent = CWsbXmlWP( _prevxml ) ;
							_prevxml->m_tag_child = wkxml ;
						}
						break ;
						
					/*
					 *	前回がタグの終了であれば、next を生成
					 *	_prevxml は brother
					 */
					case XML_READER_TYPE_END_ELEMENT :
						{
							// next の最後尾を検出
							while ( true ) {
								if ( _prevxml->m_tag_next == NULL ) break ;
								_prevxml = _prevxml->m_tag_next ;
							}
							
							// next を生成
							wkxml = CWsbXmlSP( new CWsbXml ) ;
							wkxml->m_this_week = CWsbXmlWP( wkxml ) ;
//							wkxml->m_tag_name = toString( wkname ) ;
							wkxml->m_tag_name = string( (const char*)wkname ) ;
							wkxml->m_tag_parent = CWsbXmlWP( _prevxml->m_tag_parent ) ;
							_prevxml->m_tag_next = wkxml ;
						}
						break ;
						
					/*
					 *	始めの一回だけここを通るルート
					 *	ルートデータの設定
					 *	_prevxml は root
					 */
					case XML_READER_TYPE_ROOT :
						{
							wkxml = _prevxml ;
//							wkxml->m_tag_name = toString( wkname ) ;
							wkxml->m_tag_name = string( (const char*)wkname ) ;
							wkxml->m_string = "wsb_root_xml" ;
							wkxml->m_tag_parent = CWsbXmlWP( wkxml ) ;
							
							#if WSB_XML_PRINT_DEBUG
							Print( "root ですよ" ) ;
							Print( wkxml->m_string ) ;
							#endif
						}
						break ;
						
					default :
							return WSB_XML_FAIL ;
						break ;
				}
				
				
				// 属性の取得
				int wkresult = xmlTextReaderMoveToNextAttribute( _reader ) ;
				
				if ( wkresult == WSB_XML_TRUE ) {
					
					
					wk_attr_xml = wkxml ;

					// 属性が存在した場合は属性すべて取り出す
					while ( true ) {
						
						// 属性名の取得
						if ( ( wkname = xmlTextReaderConstName( _reader ) ) == NULL ) {
							#if WSB_XML_PRINT_DEBUG
							Print( "err : 名前の取得に失敗" ) ;
							#endif
							return WSB_XML_FAIL ;
						}
						Print( wkname ) ;
						
						// 属性データの取得
						if ( ( wkvalue = xmlTextReaderConstValue( _reader ) ) == NULL ) {
							#if WSB_XML_PRINT_DEBUG
							Print( "err : 値の取得に失敗" ) ;
							#endif
							return WSB_XML_FAIL ;
						}
						
						#if WSB_XML_PRINT_DEBUG
						Print( wkvalue ) ;
						#endif
						
						// 属性の生成
						wk_add = CWsbXmlSP( new CWsbXml ) ;
						wk_add->m_this_week = CWsbXmlWP( wk_add ) ;
//						wk_add->m_tag_name	= toString( wkname ) ;
//						wk_add->m_string	= toString( wkvalue ) ;
						wk_add->m_tag_name = string( (const char*)wkname ) ;
						wk_add->m_string = string( (const char*)wkvalue ) ;

						wk_add->m_s32		= strtol( (const char*)wkvalue, NULL, 10 ) ;
						wk_add->m_f32		= (float)strtod( (const char*)wkvalue, NULL ) ;
						
						wk_attr_xml->m_attr = wk_add ;
						wk_attr_xml = wk_attr_xml->m_attr ;
						
						// 次の属性の取得
						wkresult = xmlTextReaderMoveToNextAttribute( _reader ) ;
						if ( wkresult == WSB_XML_TRUE ) {
						}
						else if ( wkresult == WSB_XML_FALSE ) {
							break ;
						}
						else if ( wkresult == WSB_XML_FAIL ) {
							#if WSB_XML_PRINT_DEBUG
							Print( "err : 属性の取得に失敗 (1)" ) ;
							#endif
							return WSB_XML_FAIL ;
						}
					}
					
					/*
					 *	空要素かどうかでの挙動の変化はここで行う
					 */
					if ( wkisempty == WSB_XML_TRUE ) {
						// 属性有りの空要素
						wknexttype	= XML_READER_TYPE_END_ELEMENT ; 
					}
					else if ( wkisempty == WSB_XML_FALSE ) {
						// 属性有りの通常要素
						wknexttype	= wknowtype ;
					}
					else if ( wkisempty == WSB_XML_FAIL ) {
						#if WSB_XML_PRINT_DEBUG
						Print( "err : 要素が空かどうかの取得に失敗" ) ;
						#endif
						return WSB_XML_FAIL ;
					}
					
				}
				else if ( wkresult == WSB_XML_FALSE ) {
					// 属性無し通常要素
					wknexttype = wknowtype ;
				}
				else if ( wkresult == WSB_XML_FAIL ) {
					#if WSB_XML_PRINT_DEBUG
					Print( "err : 属性の取得に失敗" ) ;
					#endif
					return WSB_XML_FAIL ;
				}
			}
			break ;
			
		/*
		 *	属性
		 *
		 *	ここには入ってきているのかどうかわからない
		 */
		case XML_READER_TYPE_ATTRIBUTE :
			{
				// 属性名の取得
				if ( ( wkname = xmlTextReaderConstName( _reader ) ) == NULL ) {
					#if WSB_XML_PRINT_DEBUG
					Print( "err : 名前の取得に失敗 (1)" ) ;
					#endif
					return WSB_XML_FAIL ;
				}
				#if WSB_XML_PRINT_DEBUG
				Print( wkname ) ;
				#endif
				
				// 属性データの取得
				if ( ( wkvalue = xmlTextReaderConstValue( _reader ) ) == NULL ) {
					#if WSB_XML_PRINT_DEBUG
					Print( "err : 値の取得に失敗 (1)" ) ;
					#endif
					return WSB_XML_FAIL ;
				}
				#if WSB_XML_PRINT_DEBUG
				Print( wkvalue ) ;
				Print( "属性ですよ" ) ;
				#endif
				wkxml		= _prevxml ;
				wknexttype	= _prevtype ;
			}
			break ;
			
		/*
		 *	スペース
		 *	空白データの場合は前回のデータをそのまま渡す
		 */
		case XML_READER_TYPE_WHITESPACE :
		case XML_READER_TYPE_SIGNIFICANT_WHITESPACE :
			{
				// 値の取得
				if ( ( wkvalue = xmlTextReaderConstValue( _reader ) ) == NULL ) {
					#if WSB_XML_PRINT_DEBUG
					Print( "err : 値の取得に失敗 (2)" ) ;
					#endif
					return WSB_XML_FAIL ;
				}
				
				#if WSB_XML_PRINT_DEBUG
				Print( "スペース" ) ;
				Print( wkvalue ) ;
				#endif
				
				wkxml		= _prevxml ;
				wknexttype	= _prevtype ;
			}
			break ;
			
		/*
		 *	文字列データ
		 *	_prevxml が文字列データを挿入する xml になる
		 */
		case XML_READER_TYPE_TEXT :
			{
				// 値の取得
				if ( (wkvalue = xmlTextReaderConstValue( _reader ) ) == NULL ) {
					#if WSB_XML_PRINT_DEBUG
					Print( "err : 値の取得に失敗 (3)" ) ;
					#endif
					return WSB_XML_FAIL ;
				}
				
				#if WSB_XML_PRINT_DEBUG
				Print( "文字列データの取得" ) ;
				Print( wkvalue ) ;
				#endif
				
				wkxml			= _prevxml ;
//				wkxml->m_string	= toString( wkvalue ) ;
				wkxml->m_string = string( (const char*)wkvalue ) ;

				wkxml->m_s32	= strtol( (const char*)wkvalue, NULL, 10 ) ;
				wkxml->m_f32	= (float)strtod( (const char*)wkvalue, NULL ) ;
				
				wknexttype	= wknowtype ;
			}	
			break ;
			
		/*
		 *	終了要素
		 *	前回も要素の終了であった場合は parent を返す
		 */
		case XML_READER_TYPE_END_ELEMENT :
			{
				if ( _prevtype == XML_READER_TYPE_END_ELEMENT )
					wkxml = CWsbXmlSP( _prevxml->m_tag_parent ) ;
				else
					wkxml = _prevxml ;
				
				wknexttype = wknowtype ;
			}
			break ;
			
		/*
		 *	ドキュメント
		 */
		case XML_READER_TYPE_DOCUMENT :
			{
				#if WSB_XML_PRINT_DEBUG
				Print( "ドキュメント" ) ;
				#endif
			}
			break ;
			
		/*
		 *	その他のタイプの場合は前回のデータをそのまま引き継ぐ
		 */
		default :
			{
				wkxml		= _prevxml ;
				wknexttype	= _prevtype ;
			}
			break ;
	}
	
	// 属性データの場合の処理
	if ( wknowtype == XML_READER_TYPE_ATTRIBUTE )
		return xmlTextReaderMoveToNextAttribute( _reader ) ;
	
	// 次のデータを読み込む
	int wkcompflag = xmlTextReaderRead( _reader ) ;
	if ( wkcompflag == WSB_XML_TRUE )
		return CWsbXml::ParseProcess( _reader, wkxml, wknexttype ) ;
	else if ( wkcompflag == WSB_XML_FAIL )
		return WSB_XML_FAIL ;
	
	return WSB_XML_FALSE ;

}

//=========================================================
// name... CreateXml
// work... XML を作成
// arg1... タグ名
// ret.... XML へのスマートポインタ
// tips... none
//=========================================================
CWsbXmlSP CWsbXml::CreateXml( std::string _root_name )
{
	CWsbXmlSP wkxml ;
	wkxml = CWsbXmlSP( new CWsbXml ) ;
	wkxml->m_this_week = CWsbXmlWP( wkxml ) ;
	wkxml->m_tag_name = _root_name ;
	wkxml->m_string = "wsb_root_xml" ;
	return wkxml ;
}

//=========================================================
// work... XMLをセーブ
// arg1... セーブファイル名
//=========================================================
void CWsbXml::SaveXmlFile( std::string _filename )
{
	std::string xml_str ;
	GetXmlString( &xml_str ) ;
	std::ofstream ofs( _filename ) ;
	ofs << xml_str << std::endl ;
}

//=====================================================
// テキストベースの XMLを取得
// arg1... 出力結果
// arg2... 指定しなくてOK
//=====================================================
void CWsbXml::GetXmlString( string *_string, bool start )
{
	
	if( start ){
		*_string = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
		*_string += "\n" ;
	}
	
	string esc = "\n" ;
	CWsbXmlSP sarch ;
	
	*_string += "<" + this->m_tag_name + ">" ;
	if( this->m_tag_child != NULL ){
		*_string += esc ;
		sarch = this->m_tag_child ;
		sarch->GetXmlString( _string, false ) ;
		
		// 子供に兄弟がいる場合は親が処理する
		// 兄弟に兄弟を処理させてはだめ
		if( this->m_tag_child->m_tag_next != NULL ){
			sarch = this->m_tag_child ;
			while( sarch->m_tag_next != NULL ){
				sarch = sarch->m_tag_next ;
				sarch->GetXmlString( _string, false ) ;
			}
		}
		
	}else{
		*_string += this->m_string ;
	}
	*_string += "</" + this->m_tag_name + ">" + esc ;

	return ;
}


//=========================================================
//=========================================================
void CWsbXml::Set( int _v )
{
	char str[64] ;
	memset( str, 0, sizeof(str) ) ;
	sprintf_s( str, "%d", _v ) ;
	this->m_string = str ;
	this->m_s32	= _v ;
	this->m_f32	= (float)_v ;
}

//=========================================================
//=========================================================
void CWsbXml::Set( float _v )
{

	char str[64] ;
	memset( str, 0, sizeof(str) ) ;
	sprintf_s( str, "%f", _v ) ;
	this->m_string = str ;
	this->m_s32	= (int)_v ;
	this->m_f32	= _v ;
}

//=========================================================
//=========================================================
void CWsbXml::Set( std::string _string )
{
	this->m_string = _string ;
	this->m_s32		= strtol( (const char*)_string.c_str(), NULL, 10 ) ;
	this->m_f32	= (float)strtod( (const char*)_string.c_str(), NULL ) ;
}

//=========================================================
// name... GetElementNum
// work.... エレメントの数を取得する
// arg1.... エレメント名 ( 指定なしの場合は全ての子供のエレメント )
// ret....... [ 成功 : XML へのスマートポインタ ] [ 失敗 : NULL オブジェクト ]
// tips...... none
//=========================================================
int	CWsbXml::GetElementNum( std::string _tag_name )
{
	int element_count = 0 ;
	bool all_ok = false ;
	CWsbXmlSP sarch = this->m_tag_child ;
	
	if( _tag_name == "all_element" ){
		all_ok = true ;
	}
	
	while( sarch != NULL ){
		if( all_ok ){
			element_count++ ;
		}else if( sarch->m_tag_name == _tag_name ){
			element_count++ ;
		}
		sarch = sarch->m_tag_next ;
	}
	
	return element_count ;
	
}

//=========================================================
// name... GetElement
// work.... エレメントを取得する
// arg1.... エレメント名
// arg2.... 何番目を取得するか
// ret....... [ 成功 : XML へのスマートポインタ ] [ 失敗 : NULL オブジェクト ]
// tips...... none
//=========================================================
CWsbXmlSP CWsbXml::GetElement( std::string _tag_name, int _idx )
{
	int element_count = 0 ;
	CWsbXmlSP null_obj ;
	CWsbXmlSP sarch = this->m_tag_child ;
	
	while( sarch != NULL ){
		if( sarch->m_tag_name == _tag_name ){
			if( element_count == _idx ) return sarch ;
			element_count++ ;
		}
		sarch = sarch->m_tag_next ;
	}
	
	return null_obj ;
}


//=========================================================
// name... GetElement
// work.... エレメントを取得する
// arg1.... 何番目を取得するか
// ret....... [ 成功 : XML へのスマートポインタ ] [ 失敗 : NULL オブジェクト ]
// tips...... none
//=========================================================
CWsbXmlSP CWsbXml::GetElement( int _idx )
{
	CWsbXmlSP null_obj ;
	CWsbXmlSP sarch = this->m_tag_child ;
	
	for( int i = 0 ; i < _idx ; i++ ){
		if( sarch == NULL ) return null_obj ;
		sarch = sarch->m_tag_next ;
	}
	return sarch ;
	
}


//=========================================================
// name... GetElementTop
// work.... 最上位のエレメントを取得する
// ret....... [ 成功 : XML へのスマートポインタ ] [ 失敗 : NULL オブジェクト ]
// tips...... none
//=========================================================
CWsbXmlSP CWsbXml::GetElementTop()
{
	CWsbXmlSP parent = GetParentElement() ;
	while( 1 ){
		if( parent->GetString() == "wsb_root_xml" ) return parent ;
		parent = parent->GetParentElement() ;
	}
	return parent ;
}


//=========================================================
// name... GetAttrNum
// work.... 属性の数を取得する
// arg1.... 属性名 ( 指定なしの場合は全ての属性)
// ret....... [ 成功 : XML へのスマートポインタ ] [ 失敗 : NULL オブジェクト ]
// tips...... none
//=========================================================
int	CWsbXml::GetAttrNum( std::string _tag_name )
{
	int attr_count = 0 ;
	bool all_ok = false ;
	CWsbXmlSP sarch = this->m_attr ;
	
	if( _tag_name == "all_attribute" ){
		all_ok = true ;
	}
	
	while( sarch != NULL ){
		if( all_ok ){
			attr_count++ ;
		}else if( sarch->m_tag_name == _tag_name ){
			attr_count++ ;
		}
		sarch = sarch->m_attr ;
	}
	
	return attr_count ;
	
}


//=========================================================
// name... GetAttr
// work.... 属性を取得する
// arg1.... 属性名
// arg2.... 何番目を取得するか
// ret....... [ 成功 : XML へのスマートポインタ ] [ 失敗 : NULL オブジェクト ]
// tips...... none
//=========================================================
CWsbXmlSP CWsbXml::GetAttr( std::string _tag_name, int _idx )
{
	int attr_count = 0 ;
	CWsbXmlSP null_obj ;
	CWsbXmlSP sarch = this->m_attr ;
	
	while( sarch != NULL ){
		if( sarch->m_tag_name == _tag_name ){
			if( attr_count == _idx ){
				return sarch ;
			}
			attr_count++ ;
		}
		sarch = sarch->m_attr ;
	}
	
	return null_obj ;
}

//=========================================================
// name... GetAttr
// work.... 属性を取得する
// arg1.... 何番目を取得するか
// ret....... [ 成功 : XML へのスマートポインタ ] [ 失敗 : NULL オブジェクト ]
// tips...... none
//=========================================================
CWsbXmlSP CWsbXml::GetAttr( int _idx )
{
	CWsbXmlSP null_obj ;
	CWsbXmlSP sarch = this->m_attr ;
	
	for( int i = 0 ; i < _idx ; i++ ){
		if( sarch == NULL ) return null_obj ;
		sarch = sarch->m_attr ;
	}
	return sarch ;
	
}






#pragma warning( pop )









