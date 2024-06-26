#pragma once
#include "json.h"
#include <string>
using std::string;

namespace json {
	//解析器, 存放了字符串和指针
	class Parser {
	public:
		Parser();
		
		//初始化字符串和指针
		void init(const string&);
		
		//按首字符分辨具体token的类型
		//一次只解析一个token
		//先解析n开头的, 视为parse_null类型
		//再解析t或f开头的, 视为parse_bool类型
		//再解析-和数字开头的, 视为parse_number类型
		//再解析"开头的, 视为parse_string类型
		//再解析[开头的, 视为parse_array类型
		//再解析{开头的, 视为parse_object类型
		Json json_parse();

	private:
		string m_str;
		int m_idx;

		/*以下函数运行后, 该格式的结尾字符指向下一个指针m_idx*/
		Json parse_null();
		Json parse_bool();
		Json parse_number();
		//string只需要传内容, 不需要传外侧双引号""
		//因为Json类的m_value.m_string指针指向的内容已经自动添加了双引号
		Json parse_string();
		Json parse_array();
		Json parse_object();

		//过滤
		void __skip();
		//过滤并获取下一个token的首字符
		char __get_first_char();
		//将一个token转换成string
		string __get_string();
	};
	
}