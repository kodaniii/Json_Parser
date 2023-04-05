#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <memory>

using namespace std;

namespace KARINTO{

namespace json {
	class Json {
	public:
		enum Json_type {
			json_null,
			json_bool,
			json_int,
			json_double,
			json_string,
			json_array, //数组[]
			json_object,//对象{}
		};

		Json();
		Json(bool);
		Json(int);
		Json(double);
		Json(const char*);  //string
		Json(const string&);//string
		Json(const Json_type);
		Json(const Json&);
		~Json();

		//转换函数
		operator bool();
		operator int();
		operator double();
		operator string();
		
		//重载
		Json& operator [](int); //针对数组array
		void append(const Json&);
		
		Json& operator [](const char*);//针对对象map
		Json& operator [](const string&);
		
		//以str的形式打印Json的属性和值
		string str();
		
		//重载 = 
		void operator = (const Json&);

	private:
		union Value {
			bool m_bool;
			int m_int;
			double m_double;
			string *m_string; //联合体内不能含有带复制构造、析构成员函数, 此处存放指针, 下同
			vector<Json> *m_array;		 //数组
			map<string, Json> *m_object;  //键值对
		};

		Json_type m_type;
		Value m_value;

		//返回Json的type和value, 如果type类型为null, optional=nullopt
		void __Json_get(optional<pair<string, string>>&);
		//以string形式返回Json对应value, type类型为null时不被调用, 因此忽略
		string __Json_value_string();


		//复制和清空
		//copy仅复制, 不考虑清空原有的内容
		void copy(const Json&);
		void clear();
	};
}
}
