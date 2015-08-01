#pragma once 

// コマンドを管理する
class CCommand
{
public :
	// コマンド名
	enum {
		// 無入力
		NONE = 0x00,
		// 矢印キー
		UP	 = 0x01,			// 上
		DOWN = 0x02,		// 下
		LEFT = 0x04,			// 右
		RIGHT = 0x08,		// 左
		// 各種ボタン
		CIRCLE = 0x10,		// ○
		CROSS = 0x20,		// ✕
		TRIANGLE= 0x40,	// △
		SQUARE	= 0x80,		// □
		R1 = 0x100,			// R1
		L1 = 0x200,			// L1
		R2 = 0x400,			// R2
		L2 = 0x800,			// L2
		R3 = 0x1000,			// R3
		L3 = 0x2000,			// L3
		START = 0x4000,	// スタートボタン
		SELECT = 0x8000,	// セレクトボタン
		DEBUG_ = 0x10000,	// デバッグ
	};

	// コンストラクタ
	CCommand(DWORD command = 0) : value(command) { }
	// デストラクタ
	~CCommand() { }
	
	// コマンド追加
	void Add(DWORD command) { value |= command; }
	// 指定コマンドのフラグチェック
	bool Check(size_t command){ return (value &command) == command; }
	// 移動コマンドチェック
	static bool CheckMoveCommand(CCommand &command)
	{
		return command.Check(CCommand::UP) || command.Check(CCommand::DOWN)
			|| command.Check(CCommand::LEFT) || command.Check(CCommand::RIGHT);
	}
	// XOR
	static bool CheckMoveCommandXOR(CCommand &command)
	{
		return command.Check(CCommand::UP) ^ command.Check(CCommand::DOWN) 
			^ command.Check(CCommand::LEFT) ^ command.Check(CCommand::RIGHT);
	}
	// ○✕△□コマンドチェック
	static bool CheckCCTSCommand(CCommand &command)
	{
		return command.Check(CCommand::CIRCLE) || command.Check(CCommand::CROSS)
			|| command.Check(CCommand::TRIANGLE) || command.Check(CCommand::SQUARE);
	}
	// 十字キー以外全てのコマンドチェック
	static bool CheckCommand(CCommand &command)
	{
		return command.Check(CIRCLE) || command.Check(CROSS)
			|| command.Check(TRIANGLE) || command.Check(SQUARE)
			|| command.Check(R1) || command.Check(L1)
			|| command.Check(R2) || command.Check(L2)
			|| command.Check(R3) || command.Check(L3)
			|| command.Check(START) || command.Check(SELECT);
	}

private :
	// 値(ビットフラグ)
	DWORD value;

} ;