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
		
		//支持vector和map形式的append
		void append(const Json&);
		void append(const char*, const Json&);
		void append(const string&, const Json&);
		
		Json& operator [](const char*);//针对对象map
		Json& operator [](const string&);
		
		//以str的形式打印Json的属性和值
		string str(); //以Json的形式输出
		string show_detailed(); //打印所有信息
		
		//重载
		void operator = (const Json&);
		bool operator == (const Json&);
		bool operator != (const Json&);

		bool isNull() const { return m_type == json_null; };
		bool isBool() const { return m_type == json_bool; };
		bool isInt() const { return m_type == json_int; };
		bool isDouble() const { return m_type == json_double; };
		bool isString() const { return m_type == json_string; };
		bool isArray() const { return m_type == json_array; };
		bool isObject() const { return m_type == json_object; };

		//显式转换
		bool asBool() const;
		int asInt() const;
		double asDbouble() const;
		string asString() const;

		//是否存在元素
		bool has(int);
		bool has(const char*);
		bool has(const string&);

		//移除某个元素或键值对
		bool remove(int);
		bool remove(const char*);
		bool remove(const string&);


		void parse(const string&);

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

		//array与object判断是否相等
		bool __array_is_equal(const Json&);
		bool __object_is_equal(const Json&);
	};
}
}
