#include "json.h"
#include "parser.h"
#include <sstream>
#include <string>

using namespace json;

Json::Json() : m_type(json_null) {}
Json::Json(bool value) : m_type(json_bool) {
	m_value.m_bool = value;
}
Json::Json(int value) : m_type(json_int) {
	m_value.m_int = value;
}
Json::Json(double value) : m_type(json_double) {
	m_value.m_double = value;
}
Json::Json(const char* value) : m_type(json_string) {
	m_value.m_string = new string(value);
}
Json::Json(const string& value) :m_type(json_string) {
	m_value.m_string = new string(value);
}
Json::Json(const string&& value) : m_type(json_string) {
	m_value.m_string = new string(value);
}
Json::Json(Json_type type) : m_type(type) {
	switch (m_type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = false; //只传入type, 对该type做初始化, 下同
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_string:
		m_value.m_string = new string("");
		break;
	case json_array:
		m_value.m_array = new vector<Json>();
		break;
	case json_object:
		m_value.m_object = new map<string, Json>();
		break;
	default:
		break;	
	}
}
Json::Json(const Json& rhs) {
	__copy<const Json&>(rhs);
}
Json::Json(const Json&& rhs) {
	__copy<const Json&&>(forward<const Json>(rhs));
}
Json::~Json() {
	__clear();
}
template<class T>
void Json::__copy(const T&& rhs) noexcept {
	m_type = rhs.m_type;
	switch (m_type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = rhs.m_value.m_bool;
		break;
	case json_int:
		m_value.m_int = rhs.m_value.m_int;
		break;
	case json_double:
		m_value.m_double = rhs.m_value.m_double;
		break;
	case json_string:
		//原本写的都是浅拷贝, 为了保证拷贝的指针能够正确delete, 又不想只用move(), 只好牺牲效率了
		//这样的话rhs含指针的对象引用次数只有一次, 每次析构调用一次delete即可, 也可以删除掉引用计数count变量了
		m_value.m_string = new string(*rhs.m_value.m_string); 
		break;
	case json_array:
		m_value.m_array = new vector(*rhs.m_value.m_array);
		break;
	case json_object:
		m_value.m_object = new map(*rhs.m_value.m_object);
		break;
	default:
		break;
	}
}
void Json::__clear() {
	switch (m_type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = false;
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_string:
		delete m_value.m_string;
		break;
	case json_array:
		m_value.m_array->clear();
		m_value.m_array->shrink_to_fit();
		delete m_value.m_array;
		break;
	case json_object:
		m_value.m_object->clear();
		delete m_value.m_object;
		break;
	default:
		break;
	}
	m_type = json_null;
}
Json::operator bool() {
	//先判断是否是bool类型
	if (m_type != json_bool) {
		throw new logic_error("type error, m_type is not bool.");
	}
	return m_value.m_bool;
}
Json::operator int() {
	if (m_type != json_int) {
		throw new logic_error("type error, m_type is not int.");
	}
	return m_value.m_int;
}
Json::operator double() {
	if (m_type != json_double) {
		throw new logic_error("type error, m_type is not double.");
	}
	return m_value.m_double;
}
Json::operator string() {
	if (m_type != json_string) {
		throw new logic_error("type error, m_type is not string.");
	}
	return *(m_value.m_string);
}
void Json::operator = (const Json& rhs) {
	__clear();
	__copy<const Json&>(rhs);
}
void Json::operator = (const Json&& rhs) noexcept {
	__clear();
	__copy<const Json&&>(forward<const Json>(rhs));
}
bool Json::operator == (const Json& rhs) {
	if (rhs.m_type != m_type) return false;
	switch (m_type) {
	case json_null:
		return true;
	case json_bool:
		return m_value.m_bool == rhs.m_value.m_bool;
	case json_int:
		return m_value.m_int == rhs.m_value.m_int;
	case json_double:
		return m_value.m_double == rhs.m_value.m_double;
	case json_string:
		return *(m_value.m_string) == *(rhs.m_value.m_string);
	case json_array:
		return __array_is_equal<const Json&>(rhs);
	case json_object:
		return __object_is_equal<const Json&>(rhs);
	default:
		break;
	}
	return false;
}
bool Json::operator == (const Json&& rhs) noexcept {
	if (rhs.m_type != m_type) return false;
	switch (m_type) {
	case json_null:
		return true;
	case json_bool:
		return m_value.m_bool == rhs.m_value.m_bool;
	case json_int:
		return m_value.m_int == rhs.m_value.m_int;
	case json_double:
		return m_value.m_double == rhs.m_value.m_double;
	case json_string:
		return *(m_value.m_string) == *(rhs.m_value.m_string);
	case json_array:
		return __array_is_equal<const Json&&>(forward<const Json>(rhs));
	case json_object:
		return __object_is_equal<const Json&&>(forward<const Json>(rhs));
	default:
		break;
	}
	return false;
}
bool Json::operator != (const Json& rhs) {
	return !(*this == rhs);
}
bool Json::operator != (const Json&& rhs) noexcept {
	return !(*this == rhs);
}
template<class T>
bool Json::__array_is_equal(const T&& rhs) noexcept {
	if (m_type == json_array) {
		//先判断size是否相同
		if (m_value.m_array->size() == rhs.m_value.m_array->size()) {
			auto this_it = m_value.m_array->begin();
			auto rhs_it = rhs.m_value.m_array->begin();
			while (this_it != m_value.m_array->end()) {
				if (*this_it != *rhs_it) return false;
				this_it++;
				rhs_it++;
			}
			return true;
		}
		else return false;
	}
	else throw new logic_error("__array_is_equal(): Json_type ERR.");
}
template<class T>
bool Json::__object_is_equal(const T&& rhs) {
	if (m_type == json_object) {
		if (m_value.m_object->size() == rhs.m_value.m_object->size()) {
			auto this_it = m_value.m_object->begin();
			auto rhs_it = rhs.m_value.m_object->begin();
			while (this_it != m_value.m_object->end()) {
				if (this_it->first != rhs_it->first || this_it->second != rhs_it->second) return false;
				this_it++;
				rhs_it++;
			}
			return true;
		}
		else return false;
	}
	else throw new logic_error("__object_is_equal(): Json_type ERR.");
}
Json& Json::operator [](int idx) {
	if (idx < 0) {
		throw new logic_error("idx ERR, array[] index < 0.");
	}
	if (m_type != json_array) {
		__clear();
		m_type = json_array;
		m_value.m_array = new vector<Json>();
	}
	int size = m_value.m_array->size();
	//下标大于等于size时扩容, 放入空Json
	if (idx >= size) {
		for (int i = size; i <= idx; i++) {
			m_value.m_array->push_back(Json());
		}
	}
	return m_value.m_array->at(idx);
}
Json& Json::operator [](const char* key) {
	string s(key);
	return (*this)[s]; //调用operator[](const string&)
}
Json& Json::operator [](const string& key) {
	if (m_type != json_object) {
		__clear();
		m_type = json_object;
		m_value.m_object = new map<string, Json>();
	}
	//如果map中key不存在, 直接生成默认的value
	return (*m_value.m_object)[key];
}
Json& Json::operator [](const string&& key) {
	if (m_type != json_object) {
		__clear();
		m_type = json_object;
		m_value.m_object = new map<string, Json>();
	}
	return (*m_value.m_object)[key];
}
void Json::append(const Json& rhs) {
	__append_array<const Json&>(rhs);
}
void Json::append(const Json&& rhs) noexcept {
	__append_array<const Json&&>(forward<const Json>(rhs));
}
template<class C>
void Json::__append_array(const C&& rhs) noexcept {
	if (m_type != json_array) {
		__clear();
		m_type = json_array;
		m_value.m_array = new vector<Json>();
	}
	m_value.m_array->push_back(rhs);
}
void Json::append(const char* c, const Json& rhs) {
	__append_object<const string&&, const Json&>(c, rhs);
}
void Json::append(const char* c, const Json&& rhs) {
	__append_object<const string&&, const Json&&>(c, forward<const Json>(rhs));
}
void Json::append(const string& s, const Json& rhs) {
	__append_object<const string&, const Json&>(s, rhs);
}
void Json::append(const string& s, const Json&& rhs) {
	__append_object<const string&, const Json&&>(s, forward<const Json>(rhs));
}
void Json::append(const string&& s, const Json& rhs) {
	__append_object<const string&&, const Json&>(forward<const string>(s), rhs);
}
void Json::append(const string&& s, const Json&& rhs) {
	__append_object<const string&&, const Json&&>(forward<const string>(s), forward<const Json>(rhs));
}
template<typename Ty, class C>
void Json::__append_object(const Ty&& s, const C&& rhs) {
	if (m_type != json_object) {
		__clear();
		m_type = json_object;
		m_value.m_object = new map<string, Json>();
	}
	m_value.m_object->insert(make_pair(s, rhs));
}
void Json::__Json_get(optional<pair<string, string>>& rhs) {
	stringstream ss;
	switch (m_type) {
	case json_null:
		rhs = nullopt;
		break;
	case json_bool:
		rhs = { "json_bool", __Json_value_string() };
		break;
	case json_int:
		rhs = { "json_int", __Json_value_string() };
		break;
	case json_double:
		rhs = { "json_double", __Json_value_string() };
		break;
	case json_string:
		rhs = { "json_string", __Json_value_string() };
		break;
	case json_array:
		rhs = { "json_array", __Json_value_string() };
		break;
	case json_object:
		rhs = { "json_object", __Json_value_string() };
		break;
	default:
		rhs= { "ERR_type: " + m_type, "ERR_type" };
	}
}
string Json::__Json_value_string() {
	stringstream ss;
	switch (m_type) {
	case json_bool:
		if (m_value.m_bool) ss << "true";
		else ss << "false";
		break;
	case json_int:
		ss << m_value.m_int;
		break;
	case json_double:
		ss << m_value.m_double;
		break;
	case json_string:
		//这里自动补充双引号"", 后续解析中双引号就不需要自动添加了
		ss << "\"" << *(m_value.m_string) << "\"";
		break;
	case json_array:
		ss << "[";
		for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); it++) {
			if (it != m_value.m_array->begin()) ss << ", ";
			//由于array中依然是Json, 这里使用递归
			ss << it->__Json_value_string();
		}
		ss << "]";
		break;
	case json_object:
		ss << "{";
		for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); it++) {
			if (it != m_value.m_object->begin()) ss << ", ";
			ss << "\"" << it->first << "\": " << it->second.__Json_value_string();
		}
		ss << "}";
	default:
		break;
	}
	return ss.str();
}
string Json::show_detailed() {
	stringstream ss;
	optional<pair<string, string>> optional_p;
	__Json_get(optional_p);
	if (optional_p.has_value()) {
		ss << "Json::show(): Json_type=" << optional_p.value().first
			<< ",\tJson_value=" << optional_p.value().second
			<< "." << endl;
	}
	else {
		ss << "Json::show(): Json_type=json_null." << endl;
	}
	return ss.str();
}
string Json::str() {
	stringstream ss;
	optional<pair<string, string>> optional_p;
	__Json_get(optional_p);
	if (optional_p.has_value()) {
		ss << optional_p.value().second;
	}
	ss << endl;
	return ss.str();
}
bool Json::asBool() const {
	if (m_type != json_bool) {
		throw new logic_error("type error, m_type is not bool.");
	}
	return m_value.m_bool;
}
int Json::asInt() const {
	if (m_type != json_int) {
		throw new logic_error("type error, m_type is not int.");
	}
	return m_value.m_int;
}
double Json::asDbouble() const {
	if (m_type != json_double) {
		throw new logic_error("type error, m_type is not double.");
	}
	return m_value.m_double;
}
string Json::asString() const {
	if (m_type != json_string) {
		throw new logic_error("type error, m_type is not string.");
	}
	return *(m_value.m_string);
}
bool Json::has(int idx) {
	if (m_type != json_array) {
		return false;
	}
	else {
		return (idx >= 0 && idx < (int)m_value.m_array->size());
	}
}
bool Json::has(const char* rhs) {
	return has(string(rhs));
}
bool Json::has(const string& rhs) {
	if (m_type != json_object) {
		return false;
	}
	return m_value.m_object->find(rhs) != m_value.m_object->end();
}
bool Json::has(const string&& rhs) {
	if (m_type != json_object) {
		return false;
	}
	return m_value.m_object->find(rhs) != m_value.m_object->end();
}
bool Json::remove(int idx) {
	if (isArray() && has(idx)) {
		m_value.m_array->erase(m_value.m_array->begin() + idx);
		return true;
	} 
	else return false;
}
bool Json::remove(const char* rhs) {
	return remove(string(rhs));
}
bool Json::remove(const string& rhs) {
	if (isObject() && has(rhs)) {
		m_value.m_object->erase(rhs);
		return true;
	}
	else return false;
}
bool Json::remove(const string&& rhs) {
	if (isObject() && has(rhs)) {
		m_value.m_object->erase(rhs);
		return true;
	}
	else return false;
}
void Json::parse(const char* c) {
	__parse<const string&&>(c);
}
void Json::parse(const string& rhs) {
	__parse<const string&>(rhs);
}
void Json::parse(const string&& rhs) {
	__parse<const string&&>(forward<const string>(rhs));
}
template<typename Ty>
void Json::__parse(const Ty&& rhs) {
	Parser p;
	p.init(rhs);
	*this = p.json_parse(); //返回Json对象给this指向的Json
}