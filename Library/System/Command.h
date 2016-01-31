#pragma once 

// コマンドを管理する
class Command
{
public :
	// コマンド名
	enum {
		// 無入力
		NONE = 0x00,
		// 矢印キー
		UP	 = 0x01,		// 上
		DOWN = 0x02,		// 下
		LEFT = 0x04,		// 右
		RIGHT = 0x08,		// 左
		// 各種ボタン
		CIRCLE = 0x10,		// ○
		CROSS = 0x20,		// ✕
		TRIANGLE= 0x40,		// △
		SQUARE	= 0x80,		// □
		R1 = 0x100,			// R1
		L1 = 0x200,			// L1
		R2 = 0x400,			// R2
		L2 = 0x800,			// L2
		R3 = 0x1000,		// R3
		L3 = 0x2000,		// L3
		START = 0x4000,		// スタートボタン
		SELECT = 0x8000,	// セレクトボタン
		DEBUG_ = 0x10000,	// デバッグ
	};

	// コンストラクタ
	Command(DWORD command = 0) : _value(command) { }
	// デストラクタ
	~Command() { }
	
	// コマンド追加
	void Add(DWORD command) { _value |= command; }
	// 指定コマンドのフラグチェック
	bool Contains(size_t command){ return (_value &command) == command; }
	// 移動コマンドチェック
	static bool ContainsMovement(Command &command)
	{
		return command.Contains(Command::UP) || command.Contains(Command::DOWN)
			|| command.Contains(Command::LEFT) || command.Contains(Command::RIGHT);
	}
	// XOR
	static bool ContainsMovementXOR(Command &command)
	{
		return command.Contains(Command::UP) ^ command.Contains(Command::DOWN) 
			^ command.Contains(Command::LEFT) ^ command.Contains(Command::RIGHT);
	}
	// ○✕△□コマンドチェック
	static bool ContainsCCTS(Command &command)
	{
		return command.Contains(Command::CIRCLE) || command.Contains(Command::CROSS)
			|| command.Contains(Command::TRIANGLE) || command.Contains(Command::SQUARE);
	}
	// 十字キー以外全てのコマンドチェック
	static bool Contains(Command &command)
	{
		return command.Contains(CIRCLE) || command.Contains(CROSS)
			|| command.Contains(TRIANGLE) || command.Contains(SQUARE)
			|| command.Contains(R1) || command.Contains(L1)
			|| command.Contains(R2) || command.Contains(L2)
			|| command.Contains(R3) || command.Contains(L3)
			|| command.Contains(START) || command.Contains(SELECT);
	}

private :
	// 値(ビットフラグ)
	DWORD _value;

} ;