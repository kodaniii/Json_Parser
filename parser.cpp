#include "parser.h"

#include <iostream>

using namespace KARINTO::json;

Parser::Parser() :m_str(""), m_idx(0) {}
void Parser::init(const string& rhs) {
	m_str = rhs;
	m_idx = 0;
}
void Parser::__skip() {
	while (m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\r' || m_str[m_idx] == '\t') {
		m_idx++;
	}
}
char Parser::__get_first_char() {
	__skip();
	return m_str[m_idx];
}
Json Parser::token_parse() {
	char ch = __get_first_char();
	switch(ch) {
	case 'n':
		return parse_null();
	case 't':
	case 'f':
		return parse_bool();
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return parse_number();
	case '\"':
		//注意传进去的第一位是", 这里没有过滤, 在__get_string()中过滤
		return parse_string();
	case '[':
		return parse_array();
	case '{':
		return parse_object();
	default:
		break;
	}
	throw new logic_error("Parser::parser() ERR: unexcepted char.");
}
Json Parser::parse_null() {
	if (m_str.compare(m_idx, 4, "null") == 0) {
		return Json();
	}
	throw new logic_error("Parser::parse_null() ERR: m_str is not null.");
}
Json Parser::parse_bool() {
	if (m_str.compare(m_idx, 4, "true") == 0) {
		return Json(true);
	}
	else if (m_str.compare(m_idx, 5, "false") == 0) {
		return Json(false);
	}
	throw new logic_error("Parser::parse_null() ERR: m_str is not a bool.");
}
Json Parser::parse_number() {
	//记录起始偏移量
	int pos = m_idx;
	if (m_str[m_idx] == '-') {
		m_idx++;
	}
	if (m_str[m_idx] < '0' || m_str[m_idx] > '9') {
		throw new logic_error("Parser::parse_number() ERR-1: m_str is not a number.");
	}
	while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9') {
		m_idx++;
	}
	//遍历到这里, 没有小数点, 遍历完成, 说明是整数
	if (m_str[m_idx] != '.') {
		int i = atoi(m_str.c_str() + pos); //m_str首指针加偏移量pos
		return Json(i);
	}
	//有小数点
	else {
		m_idx++;
		if (m_str[m_idx] < '0' || m_str[m_idx] > '9') {
			throw new logic_error("Parser::parse_number() ERR-2: m_str is not a number.");
		}
		while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9') {
			m_idx++;
		}
		//遍历到这里, 小数部分遍历完成, 是浮点数
		double f = atof(m_str.c_str() + pos);
		return Json(f);
	}
}
string Parser::__get_string() {
	string s;
	m_idx++; //过滤传入的第一个"字符
	while (true) {
		char ch = m_str[m_idx++]; //初始状态下, 获取token的第一个字符
		//""或遍历到第二个"的情况
		if (ch == '\"') {
			break;
		}
		//转义
		if (ch == '\\') {
			ch = m_str[m_idx++];
			switch (ch) {
			case '\"':
			case '\\':
			case '\/':
			case '\b':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
				s += "\\"; //符合上述转义, 加入上个字符转义前的内容
				s += ch;   //加入该字符
				break;
			//'\u'会报错, 就只检查u好了
			//\u的特点是继续接收后4个字符
			case 'u':
				s += "\\u";
				for (int i = 0; i < 4; i++) {
					s += m_str[m_idx++];
				}
				break;
			default:
				s += '\\'; //没有被转义的情况
				s += ch;
				break;
			}
		}
		else {
			s += ch;
		}
	}
	return s;
}
Json Parser::parse_string() {
	string s = __get_string();
	return Json(s);
}
Json Parser::parse_array() {
	return Json();
}
Json Parser::parse_object() {
	return Json();
}