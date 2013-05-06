#include <lexer.hpp>
using namespace std;
namespace TokenType = Enum::Lexer::Token;
namespace SyntaxType = Enum::Lexer::Syntax;
namespace TokenKind = Enum::Lexer;

Scanner::Scanner() :
	isStringStarted(false), isRegexStarted(false), isPrototypeStarted(false), commentFlag(false), hereDocumentFlag(false),
	regex_delim(0), regex_middle_delim(0),
    brace_count_inner_regex(0), bracket_count_inner_regex(0), cury_brace_count_inner_regex(0)
{
	regex_prefix_map.insert(StringMap::value_type("q", ""));
	regex_prefix_map.insert(StringMap::value_type("qq", ""));
	regex_prefix_map.insert(StringMap::value_type("qw", ""));
	regex_prefix_map.insert(StringMap::value_type("qx", ""));
	regex_prefix_map.insert(StringMap::value_type("qr", ""));
	regex_prefix_map.insert(StringMap::value_type("m", ""));

	regex_replace_map.insert(StringMap::value_type("s", ""));
	regex_replace_map.insert(StringMap::value_type("y", ""));
	regex_replace_map.insert(StringMap::value_type("tr", ""));

	operator_map.insert(StringMap::value_type("<=>", ""));
	operator_map.insert(StringMap::value_type("**=", ""));
	operator_map.insert(StringMap::value_type("//=", ""));
	operator_map.insert(StringMap::value_type("||=", ""));
	operator_map.insert(StringMap::value_type("&&=", ""));
	operator_map.insert(StringMap::value_type("...", ""));
	operator_map.insert(StringMap::value_type("$#{", ""));
	operator_map.insert(StringMap::value_type("$^A", ""));
	operator_map.insert(StringMap::value_type("$^D", ""));
	operator_map.insert(StringMap::value_type("$^E", ""));
	operator_map.insert(StringMap::value_type("$^F", ""));
	operator_map.insert(StringMap::value_type("$^G", ""));
	operator_map.insert(StringMap::value_type("$^H", ""));
	operator_map.insert(StringMap::value_type("$^I", ""));
	operator_map.insert(StringMap::value_type("$^L", ""));
	operator_map.insert(StringMap::value_type("$^M", ""));
	operator_map.insert(StringMap::value_type("$^O", ""));
	operator_map.insert(StringMap::value_type("$^P", ""));
	operator_map.insert(StringMap::value_type("$^R", ""));
	operator_map.insert(StringMap::value_type("$^T", ""));
	operator_map.insert(StringMap::value_type("$^W", ""));
	operator_map.insert(StringMap::value_type("$^X", ""));

	operator_map.insert(StringMap::value_type("<=", ""));
	operator_map.insert(StringMap::value_type(">=", ""));
	operator_map.insert(StringMap::value_type(".=", ""));
	operator_map.insert(StringMap::value_type("!=", ""));
	operator_map.insert(StringMap::value_type("==", ""));
	operator_map.insert(StringMap::value_type("+=", ""));
	operator_map.insert(StringMap::value_type("-=", ""));
	operator_map.insert(StringMap::value_type("*=", ""));
	operator_map.insert(StringMap::value_type("%=", ""));
	operator_map.insert(StringMap::value_type("|=", ""));
	operator_map.insert(StringMap::value_type("&=", ""));
	operator_map.insert(StringMap::value_type("^=", ""));
	operator_map.insert(StringMap::value_type("<<", ""));
	operator_map.insert(StringMap::value_type(">>", ""));
	operator_map.insert(StringMap::value_type("++", ""));
	operator_map.insert(StringMap::value_type("--", ""));
	operator_map.insert(StringMap::value_type("**", ""));
	operator_map.insert(StringMap::value_type("//", ""));
	operator_map.insert(StringMap::value_type("&&", ""));
	operator_map.insert(StringMap::value_type("||", ""));
	operator_map.insert(StringMap::value_type("::", ""));
	operator_map.insert(StringMap::value_type("..", ""));
	operator_map.insert(StringMap::value_type("=>", ""));
	operator_map.insert(StringMap::value_type("->", ""));
	operator_map.insert(StringMap::value_type("@{", ""));
	operator_map.insert(StringMap::value_type("%{", ""));
	operator_map.insert(StringMap::value_type("${", ""));
	operator_map.insert(StringMap::value_type("@$", ""));
	operator_map.insert(StringMap::value_type("%$", ""));
	operator_map.insert(StringMap::value_type("&$", ""));
	operator_map.insert(StringMap::value_type("$#", ""));
	operator_map.insert(StringMap::value_type("<>", ""));
	operator_map.insert(StringMap::value_type("!~", ""));
	operator_map.insert(StringMap::value_type("~~", ""));
	operator_map.insert(StringMap::value_type("=~", ""));

	operator_map.insert(StringMap::value_type("$0", ""));
	operator_map.insert(StringMap::value_type("$1", ""));
	operator_map.insert(StringMap::value_type("$2", ""));
	operator_map.insert(StringMap::value_type("$3", ""));
	operator_map.insert(StringMap::value_type("$4", ""));
	operator_map.insert(StringMap::value_type("$5", ""));
	operator_map.insert(StringMap::value_type("$6", ""));
	operator_map.insert(StringMap::value_type("$7", ""));
	operator_map.insert(StringMap::value_type("$8", ""));
	operator_map.insert(StringMap::value_type("$9", ""));

	operator_map.insert(StringMap::value_type("$&", ""));
	operator_map.insert(StringMap::value_type("$`", ""));
	operator_map.insert(StringMap::value_type("$'", ""));
	operator_map.insert(StringMap::value_type("$+", ""));
	operator_map.insert(StringMap::value_type("$.", ""));
	operator_map.insert(StringMap::value_type("$/", ""));
	operator_map.insert(StringMap::value_type("$|", ""));
	operator_map.insert(StringMap::value_type("$,", ""));
	operator_map.insert(StringMap::value_type("$\\", ""));
	operator_map.insert(StringMap::value_type("$\"", ""));
	operator_map.insert(StringMap::value_type("$%", ""));
	operator_map.insert(StringMap::value_type("$=", ""));
	operator_map.insert(StringMap::value_type("$-", ""));
	operator_map.insert(StringMap::value_type("$~", ""));
	operator_map.insert(StringMap::value_type("$^", ""));
	operator_map.insert(StringMap::value_type("$*", ""));
	operator_map.insert(StringMap::value_type("$:", ""));
	operator_map.insert(StringMap::value_type("$;", ""));
	operator_map.insert(StringMap::value_type("$?", ""));
	operator_map.insert(StringMap::value_type("$!", ""));
	operator_map.insert(StringMap::value_type("$@", ""));
	operator_map.insert(StringMap::value_type("$$", ""));
	operator_map.insert(StringMap::value_type("$<", ""));
	operator_map.insert(StringMap::value_type("$>", ""));
	operator_map.insert(StringMap::value_type("$(", ""));
	operator_map.insert(StringMap::value_type("$)", ""));
	operator_map.insert(StringMap::value_type("$[", ""));
	operator_map.insert(StringMap::value_type("$]", ""));
}

Token *Scanner::scanQuote(LexContext *ctx, char quote)
{
	Token *ret = NULL;
	char *token = ctx->buffer();
	if (isStringStarted) {
		ret = new Token(string(token), ctx->finfo);
		switch (quote) {
		case '\'':
			ret->info = getTokenInfo(TokenType::RawString);
			break;
		case '"':
			ret->info = getTokenInfo(TokenType::String);
			break;
		case '`':
			ret->info = getTokenInfo(TokenType::ExecString);
			break;
		default:
			break;
		}
		ctx->clearBuffer();
		Token *prev_tk = ctx->tmgr->tokens->lastToken();
		if (prev_tk && prev_tk->data == "<<") {
			/* String is HereDocument */
			here_document_tag = ret->data;
			if (here_document_tag == "") {
				here_document_tag = "\n";
			}
			switch (quote) {
			case '\'':
				ret->info = getTokenInfo(TokenType::HereDocumentRawTag);
				break;
			case '"':
				ret->info = getTokenInfo(TokenType::HereDocumentTag);
				break;
			default:
				break;
			}
		}
		isStringStarted = false;
	} else {
		start_string_ch = quote;
		isStringStarted = true;
	}
	return ret;
}

bool Scanner::scanNegativeNumber(LexContext *ctx, char number)
{
	char num_buffer[2] = {0};
	if (number != EOL && !isStringStarted) {
		num_buffer[0] = number;
		if (atoi(num_buffer) > 0 || number == '0') {
			//negative number
			ctx->writeBuffer('-');
			return true;
		}
	}
	return false;
}

bool Scanner::isRegexStartDelim(LexContext *ctx, const StringMap &map)
{
	/* exclude { m } or { m => ... } or { m, ... } or *m or //m */
	Token *before_prev_token = ctx->tmgr->tokens->lastToken();
	string before_prev_data = (before_prev_token) ? before_prev_token->data : "";
	TokenType::Type before_prev_type = (before_prev_token) ?
		before_prev_token->info.type : TokenType::Undefined;
	string prev_data = string(ctx->buffer());
	char symbol = ctx->smgr->currentChar();
	//... [before_prev_token] [prev_token] [symbol] ...
	if (before_prev_type == TokenType::RegDelim) return false; /* regex option */
	if (before_prev_data == "*") return false;  /* glob */
	if (before_prev_data == "&") return false;  /* function call */
	if (before_prev_data == "::") return false; /* method call */
	if (symbol == '}' || symbol == ',' || symbol == '=') return false;
	if (map.find(prev_data) != map.end()) return true;
	return false;
}

bool Scanner::isRegexEndDelim(LexContext *ctx)
{
	Token *token = ctx->tmgr->tokens->lastToken();
	TokenType::Type type = (token) ? token->info.type : TokenType::Undefined;
	if (isRegexStarted) return true;
	if (type == TokenType::RegExp) return true;
	if (type == TokenType::RegReplaceTo) return true;
	return false;
}

char Scanner::getRegexDelim(LexContext *ctx)
{
	char ret = EOL;
	char symbol = ctx->smgr->currentChar();
	switch (symbol) {
	case '{':
		ret = '}';
		brace_count_inner_regex++;
		break;
	case '(':
		ret = ')';
		cury_brace_count_inner_regex++;
		break;
	case '[':
		ret = ']';
		bracket_count_inner_regex++;
		break;
	case '<':
		ret = '>';
		break;
	default:
		ret = symbol;
		break;
	}
	return ret;
}

bool Scanner::isPrototype(LexContext *ctx)
{
	Token *prev_token = ctx->tmgr->tokens->lastToken();
	string prev_data = (prev_token) ? prev_token->data : "";
	int idx = ctx->tmgr->tokens->size() - 2;
	string before_prev_data = (idx >= 0) ? ctx->tmgr->tokens->at(idx)->data : "";
	char symbol = ctx->smgr->currentChar();
	if (symbol != '(') return false;
	if (prev_data == "sub") return true;
	if (before_prev_data == "sub") return true;
	return false;
}

bool Scanner::isHereDocument(LexContext *ctx, Token *tk)
{
	string tk_data = (tk) ? tk->data : "";
	char *token = ctx->buffer();
	if (tk_data == "<<" &&
		strtod(token, NULL) == 0 && string(token) != "0" &&
		(isupper(token[0]) || token[0] == '_')) {
		return true;
	}
	return false;
}

bool Scanner::isRegexDelim(Token *prev_token, char symbol)
{
	const char *prev_data = (prev_token) ? cstr(prev_token->data) : "";
	/* [^0-9] && !"0" && !CONST && !{hash} && ![array] && !func() && !$var */
	string prev_tk = string(prev_data);
	if (regex_delim == 0 && prev_token && prev_token->info.type == TokenType::Undefined &&
		(symbol != '-' && symbol != '=' && symbol != ',') &&
		(prev_tk == "q"  || prev_tk == "qq" || prev_tk == "qw" ||
		 prev_tk == "qx" || prev_tk == "qr" || prev_tk == "m")) {
		return true;
	}
	if (symbol == '/' && (prev_tk == "xor" || prev_tk == "and")) return true;
	if (symbol != '/') return false;
	if (!prev_token) return true;
	if (strtod(prev_data, NULL)) return false;
	if (string(prev_data) == "0") return false;
	if (!isupper(prev_data[0]) && prev_data[0] != '_' &&
		prev_data[0] != '}' && prev_data[0] != ']' && prev_data[0] != ')' &&
		prev_data[0] != '$' && prev_data[0] != '@' && prev_data[0] != '%') {
		if (isalpha(prev_data[0]) && string(prev_data) != "split" &&
			string(prev_data) != "if" && string(prev_data) != "unless") return false;
		return true;
	}
	if (string(prev_data) == "split") return true;
	return false;
}

Token *Scanner::scanPrevSymbol(LexContext *ctx, char symbol)
{
	char *token = ctx->buffer();
	Token *ret = new Token(string(token), ctx->finfo);
	if (isRegexStartDelim(ctx, regex_prefix_map)) {
		//RegexPrefix
		ret->info = getTokenInfo(token);
		regex_delim = getRegexDelim(ctx);
		isRegexStarted = true;
	} else if (isRegexStartDelim(ctx, regex_replace_map)) {
		//ReplaceRegexPrefix
		ret->info = getTokenInfo(token);
		char delim = getRegexDelim(ctx);
		regex_delim = delim;
		regex_middle_delim = delim;
		isRegexStarted = true;
	} else if (isPrototype(ctx)) {
		isPrototypeStarted = true;
	} else {
		Token *prev_before_tk = ctx->tmgr->tokens->lastToken();
		if (isHereDocument(ctx, prev_before_tk)) {
			/* Key is HereDocument */
			here_document_tag = token;
			ret->info = getTokenInfo(TokenType::HereDocumentRawTag);
		}
	}
	ctx->clearBuffer();
	return ret;
}

Token *Scanner::scanCurSymbol(LexContext *ctx, char symbol)
{
	Token *ret = NULL;
	char *token = ctx->buffer();
	char tmp[2] = {0};
	tmp[0] = symbol;
	Token *prev_tk = ctx->tmgr->tokens->lastToken();
	int idx = ctx->tmgr->tokens->size() - 2;
	string prev_before = (idx >= 0) ? ctx->tmgr->tokens->at(idx)->data : "";
	if (prev_before != "sub" && isRegexDelim(prev_tk, symbol)) {
		ret = new Token(string(tmp), ctx->finfo);
		ret->info = getTokenInfo(TokenType::RegDelim);
		ctx->clearBuffer();
		if (!isRegexEndDelim(ctx)) {
			regex_delim = getRegexDelim(ctx);
			isRegexStarted = true;
		} else {
			regex_delim = 0;
		}
	} else if (isRegexEndDelim(ctx)) {
		ret = new Token(string(tmp), ctx->finfo);
		ret->info = getTokenInfo(TokenType::RegDelim);
		ctx->clearBuffer();
	} else if (symbol == '@' || symbol == '$' || symbol == '%') {// || symbol == '&') {
		ctx->writeBuffer(symbol);
	} else if (symbol == ';') {
		ret = new Token(string(tmp), ctx->finfo);
		ctx->clearBuffer();
	} else if (isPrototype(ctx)) {
		ret = new Token(string(tmp), ctx->finfo);
		ctx->clearBuffer();
		isPrototypeStarted = true;
	} else {
		ret = new Token(string(tmp), ctx->finfo);
		ctx->clearBuffer();
	}
	return ret;
}

Token *Scanner::scanTripleCharacterOperator(LexContext *ctx, char symbol, char next_ch, char after_next_ch)
{
	Token *ret = NULL;
	char tmp[4] = {0};
	tmp[0] = symbol;
	tmp[1] = next_ch;
	tmp[2] = after_next_ch;
	if (operator_map.find(string(tmp)) != operator_map.end()) {
		ret = new Token(string(tmp), ctx->finfo);
		ctx->progress = 2;
	}
	return ret;
}

Token *Scanner::scanDoubleCharacterOperator(LexContext *ctx, char symbol, char next_ch)
{
	Token *ret = NULL;
	char tmp[3] = {0};
	tmp[0] = symbol;
	tmp[1] = next_ch;
	if (operator_map.find(string(tmp)) != operator_map.end()) {
		ret = new Token(string(tmp), ctx->finfo);
		ctx->progress = 1;
	} else if (symbol == '/' && next_ch == '=') {
		Token *prev_tk = ctx->tmgr->tokens->lastToken();
		const char *prev_data = cstr(prev_tk->data);
		/* '/=' is RegDelim + RegExp or DivEqual */
		if (strtod(prev_data, NULL) != 0 || string(prev_data) == "0" || isupper(prev_data[0]) ||
			prev_data[0] == '}' || prev_data[0] == ']' ||
			prev_data[0] == ')' || prev_data[0] == '$') {
			tmp[0] = symbol;
			tmp[1] = next_ch;
			ret = new Token(string(tmp), ctx->finfo);
			ctx->progress = 1;
		}
	}
	return ret;
}

Token *Scanner::scanSymbol(LexContext *ctx, char symbol, char next_ch, char after_next_ch)
{
	Token *ret = NULL;
	if (ctx->existsBuffer()) ctx->tmgr->tokens->push_back(scanPrevSymbol(ctx, symbol));
	if (!isRegexStarted) {
		ret = scanTripleCharacterOperator(ctx, symbol, next_ch, after_next_ch);
		if (!ret) ret = scanDoubleCharacterOperator(ctx, symbol, next_ch);
	}
	if (!ret) ret = scanCurSymbol(ctx, symbol);
	return ret;
}

#define NEXT() (*(src + i++))
#define PREDICT() (*(src + i))
Token *Scanner::scanNumber(LexContext *ctx)
{
	char *src = ctx->smgr->raw_script;
	size_t i = ctx->smgr->idx;
	char *begin = src + i;
	int c = NEXT();
	Token *token = NULL;
	assert((c == '.' || ('0' <= c && c <= '9')) && "It do not seem as Number");
	bool isFloat = false;
	if ('0' <= c && c <= '9') {
		/* first char */
		if ('0' <= c && c <= '9' && c != EOL) c = NEXT();
		/* second char is includes 'b' or 'x' */
		if ((('0' <= c && c <= '9') || c == 'b' || c == 'x') && c != EOL) c = NEXT();
		for (;(('0' <= c && c <= '9') ||
			   ('a' <= c && c <= 'f') ||
			   ('A' <= c && c <= 'F')) && c != EOL; c = NEXT()) {}
	}
	if (c != '.' && c != 'e' && c != 'E') {
		goto L_emit;
	}
	if (c == '.') {
		c = PREDICT();
		if (c == '.') {
			goto L_emit; /* Number .. */
		}
		isFloat = true;
		for (; '0' <= c && c <= '9' && c != EOL; c = NEXT()) {}
	}
	if (c == 'e' || c == 'E') {
		isFloat = true;
		c = NEXT();
		if (c == '+' || c == '-') {
			c = NEXT();
		}
		for (; '0' <= c && c <= '9' && c != EOL; c = NEXT()) {}
	}
	L_emit:;
	i -= 1;
	token = new Token(string(begin, src+i), ctx->finfo);
	token->info = isFloat ? getTokenInfo(TokenType::Double) : getTokenInfo(TokenType::Int);
	ctx->smgr->idx = --i;
	return token;
}
#undef NEXT
#undef PREDICT

bool Scanner::isSkip(LexContext *ctx)
{
	using namespace Enum::Lexer::Token;
	bool ret = commentFlag;
	ScriptManager *smgr = ctx->smgr;
	char *script = smgr->raw_script;
	size_t idx = smgr->idx;
	if (smgr->previousChar() == '\n' && smgr->currentChar() == '=' &&
		isalnum(smgr->nextChar())) {
		if (smgr->compare(1, 3, "cut")) {
			DBG_PL("commentFlag => OFF");
			ctx->progress = 4;
			commentFlag = false;
			ret = false;
			ctx->finfo.start_line_num++;
		} else {
			DBG_PL("commentFlag => ON");
			commentFlag = true;
			ret = true;
		}
	} else if (here_document_tag != "__END__" && smgr->previousChar() == '\n' &&
			   smgr->compare(0, 7, "__END__")) {
		int progress_to_end = ctx->script_size - idx - 1;
		ctx->progress = progress_to_end;
		ret = false;
	} else if (here_document_tag != "__DATA__" && smgr->previousChar() == '\n' &&
			   smgr->compare(0, 8, "__DATA__")) {
		int progress_to_end = ctx->script_size - idx - 1;
		ctx->progress = progress_to_end;
		ret = false;
    } else if (isRegexStarted) {
		if (smgr->previousChar() != '\\' ||
			(smgr->previousChar() == '\\' && smgr->beforePreviousChar() == '\\')) {
			switch (script[idx]) {
			case '{': brace_count_inner_regex++;
				break;
			case '}': brace_count_inner_regex--;
				break;
			case '[': bracket_count_inner_regex++;
				break;
			case ']': bracket_count_inner_regex--;
				break;
			case '(': cury_brace_count_inner_regex++;
				break;
			case ')': cury_brace_count_inner_regex--;
				break;
			default:
				break;
			}
		}
		if (smgr->previousChar() == '\\' && smgr->beforePreviousChar() != '\\') {
			ctx->writeBuffer(script[idx]);
			ret = true;
		} else if (script[idx] != regex_delim && script[idx] != regex_middle_delim) {
			ctx->writeBuffer(script[idx]);
			ret = true;
		} else if (script[idx] == regex_middle_delim) {
			if ((regex_middle_delim == '}' && brace_count_inner_regex != 0) ||
				(regex_middle_delim == ')' && cury_brace_count_inner_regex != 0) ||
				(regex_middle_delim == ']' && bracket_count_inner_regex != 0)) {
				ctx->writeBuffer(script[idx]);
				ret = true;
			} else {
				Token *tk = NULL;
				if (regex_middle_delim != '{' &&
					regex_middle_delim != '(' &&
					regex_middle_delim != '<' &&
					regex_middle_delim != '[') {
					tk = new Token(string(ctx->buffer()), ctx->finfo);
					tk->info = getTokenInfo(RegReplaceFrom);
					ctx->clearBuffer();
					ctx->tmgr->tokens->push_back(tk);
				}
				char tmp[2] = {regex_middle_delim, 0};
				tk = new Token(string(tmp), ctx->finfo);
				tk->info = getTokenInfo(RegMiddleDelim);
				ctx->tmgr->tokens->push_back(tk);
				switch (regex_middle_delim) {
				case '}':
					regex_middle_delim = '{';
					break;
				case ')':
					regex_middle_delim = '(';
					break;
				case '>':
					regex_middle_delim = '<';
					break;
				case ']':
					regex_middle_delim = '[';
					break;
				default:
					regex_middle_delim = '\0';
					break;
				}
				ret = true;
			}
		} else {
			if ((regex_delim == '}' && brace_count_inner_regex != 0) ||
				(regex_delim == ')' && cury_brace_count_inner_regex != 0) ||
				(regex_delim == ']' && bracket_count_inner_regex != 0)) {
				ctx->writeBuffer(script[idx]);
				ret = true;
			} else {
				Token *tk = new Token(string(ctx->buffer()), ctx->finfo);
				Token *prev_tk = ctx->tmgr->tokens->back();
				tk->info = (prev_tk->info.type == RegMiddleDelim) ? getTokenInfo(RegReplaceTo) : getTokenInfo(RegExp);
				ctx->clearBuffer();
				ctx->tmgr->tokens->push_back(tk);
				ret = false;
				isRegexStarted = false;
				regex_delim = 0;
				brace_count_inner_regex = 0;
				cury_brace_count_inner_regex = 0;
				bracket_count_inner_regex = 0;
			}
		}
	} else if (isPrototypeStarted) {
		if (script[idx] == ')') {
			Token *tk = new Token(string(ctx->buffer()), ctx->finfo);
			tk->info = getTokenInfo(Prototype);
			ctx->clearBuffer();
			ctx->tmgr->tokens->push_back(tk);
			isPrototypeStarted = false;
			ret = false;
		} else {
			ctx->writeBuffer(script[idx]);
			ret = true;
		}
	} else if (isStringStarted) {
		if (script[idx] == start_string_ch &&
			((smgr->previousChar() == '\\' && smgr->beforePreviousChar() == '\\') ||
			 smgr->previousChar() != '\\')) {
			ret = false;
		} else {
			ctx->writeBuffer(script[idx]);
			ret = true;
		}
	} else if (hereDocumentFlag) {
		size_t len = here_document_tag.size();
		if (smgr->previousChar() == '\n' && idx + len < ctx->script_size) {
			size_t i;
			for (i = 0; i < len && script[idx + i] == here_document_tag.at(i); i++) {}
			if (i == len) {
				ctx->progress = len;
				Token *tk = new Token(string(ctx->buffer()), ctx->finfo);
				tk->info = getTokenInfo(TokenType::HereDocument);
				ctx->clearBuffer();
				ctx->tmgr->tokens->push_back(tk);
				tk = new Token(here_document_tag, ctx->finfo);
				tk->info = getTokenInfo(TokenType::HereDocumentEnd);
				ctx->tmgr->tokens->push_back(tk);
				ctx->finfo.start_line_num++;
				here_document_tag = "";
				hereDocumentFlag = false;
				ret = false;
			} else {
				ctx->writeBuffer(script[idx]);
				ret = true;
			}
		} else {
			ctx->writeBuffer(script[idx]);
			ret = true;
		}
	}
	return ret;
}

TokenInfo Scanner::getTokenInfo(TokenType::Type type)
{
	size_t i = 0;
	for (; decl_tokens[i].type != TokenType::Undefined; i++) {
		if (type == decl_tokens[i].type) {
			return decl_tokens[i];
		}
	}
	return decl_tokens[i];
}

TokenInfo Scanner::getTokenInfo(const char *data)
{
	size_t i = 0;
	size_t dsize = strlen(data);
	for (; decl_tokens[i].type != TokenType::Undefined; i++) {
		const char *token_data = decl_tokens[i].data;
		size_t tsize = strlen(token_data);
		if (dsize == tsize && !strncmp(token_data, data, dsize)) {
			return decl_tokens[i];
		}
	}
	return decl_tokens[i];
}
