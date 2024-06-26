#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <memory>

using namespace std;

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
		Json(const string&&);
		Json(const Json_type);
		Json(const Json&);
		Json(const Json&&);
		~Json();

		//转换函数
		operator bool();
		operator int();
		operator double();
		operator string();
		
		//重载
		Json& operator [](int); //针对数组array
		
		//支持vector和map形式的append
		//vector/array
		void append(const Json&);
		void append(const Json&&) noexcept;
		//map/object
		void append(const char*, const Json&);
		void append(const char*, const Json&&);
		void append(const string&, const Json&);
		void append(const string&, const Json&&);
		void append(const string&&, const Json&);
		void append(const string&&, const Json&&);
		
		Json& operator [](const char*);//针对对象map
		Json& operator [](const string&);
		Json& operator [](const string&&);

		//以str的形式打印Json的属性和值
		string str(); //以Json的形式输出
		string show_detailed(); //打印所有信息
		
		//重载
		void operator = (const Json&);
		void operator = (const Json&&) noexcept;
		bool operator == (const Json&);
		bool operator == (const Json&&) noexcept;
		bool operator != (const Json&);
		bool operator != (const Json&&) noexcept;

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
		bool has(const string&&);

		//移除某个元素或键值对
		bool remove(int);
		bool remove(const char*);
		bool remove(const string&);
		bool remove(const string&&);

		//新建Parser对象, 初始化并调用json解析函数, 返回解析结果给Json类
		void parse(const char*);
		void parse(const string&);
		void parse(const string&&);

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

		//修改rhs的值为Json的type和value, 如果type类型为null, optional=nullopt
		void __Json_get(optional<pair<string, string>>&);
		//以string形式返回Json对应value, type类型为null时不被调用, 因此忽略
		string __Json_value_string();


		//复制和清空
		//copy仅复制, 不考虑清空原有的内容
		template<class T>
		void __copy(const T&&) noexcept;
		void __clear();

		//array与object判断是否相等
		template<class T>
		bool __array_is_equal(const T&&) noexcept;
		template<class T>
		bool __object_is_equal(const T&&);

		//append_array
		template<class C>
		void __append_array(const C&&) noexcept;
		//append_object
		template<typename Ty, class C>
		void __append_object(const Ty&&, const C&&);
		
		//parse
		template<typename Ty>
		void __parse(const Ty&&);
	};
}
