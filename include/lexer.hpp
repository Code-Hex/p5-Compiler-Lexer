#include "common.hpp"
#include "gen_token.hpp"

namespace Enum {
	namespace Lexer {
		namespace Char {
			typedef enum {
				DoubleQuote = '"',
				Hash        = '#',
				Space       = ' ',
				Tab         = '\t',
				BackSlash   = '\\',
				Amp         = '&',
			} Type;
		}
		namespace Syntax {
			typedef enum {
				Value,
				Term,
				Expr,
				Stmt,
				BlockStmt
			} Type;
		}
	}
}

class FileInfo {
public:
	size_t start_line_num;
	size_t end_line_num;
	const char *filename;
};

class TokenInfo {
public:
	Enum::Lexer::Token::Type type;
	Enum::Lexer::Kind kind;
	const char *name;
	const char *data;
};

class Token {
public:
	Enum::Lexer::Syntax::Type stype;
	Enum::Lexer::Token::Type type;
	TokenInfo info;
	FileInfo finfo;
	Token **tks;
	std::string data;
	int idx;
	int indent;
	size_t token_num;
	size_t total_token_num;
	std::string deparsed_data;
	bool isDeparsed;

	Token(std::string data_, FileInfo finfo);
	Token(Tokens *tokens);
	const char *deparse(void);
};

extern TokenInfo decl_tokens[];
class LexContext {
public:
	char *token;
	size_t max_token_size;
	int token_idx;
	Tokens *tokens;
	int progress;
};

class Lexer {
public:
	bool isStringStarted;
	bool isRegexStarted;
	bool commentFlag;
	TokenPos start_pos;
	TokenPos pos;
	FileInfo finfo;
	char start_string_ch;
	char regex_delim;
	char regex_middle_delim;

	Lexer(const char *filename);
	Tokens *tokenize(char *script);
	void annotateTokens(Tokens *tokens);
	void grouping(Tokens *tokens); /* for Namespace::Namespace */
	void prepare(Tokens *tokens);
	Token *parseSyntax(Token *start_token, Tokens *tokens);
	void dump(Tokens *tokens);
	void dumpSyntax(Token *tk, int indent);
	Tokens *getTokensBySyntaxLevel(Token *root, Enum::Lexer::Syntax::Type type);
	Tokens *getUsedModules(Token *root);
private:
	bool isSkip(LexContext *ctx, char *script, size_t idx);
	void writeChar(LexContext *ctx, char *token, char ch);
	void clearToken(LexContext *ctx, char *token);
	Token *scanQuote(LexContext *ctx, char quote);
	//Token *scanEscapeChar(LexContext *ctx, char ch);
	Token *scanNewLineKeyword(LexContext *ctx);
	Token *scanTabKeyword(LexContext *ctx);
	Token *scanPrevSymbol(LexContext *ctx, char symbol);
	Token *scanCurSymbol(LexContext *ctx, char symbol);
	Token *scanDoubleCharacterOperator(LexContext *ctx, char symbol, char next_ch);
	Token *scanTripleCharacterOperator(LexContext *ctx, char symbol, char next_ch, char after_next_ch);
	Token *scanSymbol(LexContext *ctx, char symbol);
	Token *scanSymbol(LexContext *ctx, char symbol, char next_ch);
	Token *scanSymbol(LexContext *ctx, char symbol, char next_ch, char after_next_ch);
	Token *scanNumber(LexContext *ctx, char *src, size_t &i);
	bool scanNegativeNumber(LexContext *ctx, char num);
	TokenInfo getTokenInfo(Enum::Lexer::Token::Type type);
	TokenInfo getTokenInfo(const char *data);
	bool search(std::vector<std::string> list, std::string str);
	void insertParenthesis(Tokens *tokens);
	bool isReservedKeyword(std::string word);
};

#define isSKIP() commentFlag
extern void *safe_malloc(size_t size);
extern void safe_free(void *ptr, size_t size);
