#include <iostream>
#include "json.h"

using std::cout;
using std::endl;
using namespace KARINTO::json;

int main() {
	Json tmp_v1;
	Json tmp_v2 = true;
	Json tmp_v3 = 123;
	Json tmp_v4 = 128.33;
	Json tmp_v5 = "hello world~";

	bool tmp_b = tmp_v2;
	int tmp_i = tmp_v3;
	double tmp_d = tmp_v4;
	const string& tmp_s = tmp_v5;
	

	Json tmp_arr;
	tmp_arr[0] = true;
	tmp_arr[1] = 123;
	
	tmp_arr.append(128.33);
	tmp_arr.append("nice day~");
	
	bool tmp_b2 = tmp_arr[0];
	int tmp_i2 = tmp_arr[1];
	double tmp_d2 = tmp_arr[2];
	string tmp_s2 = tmp_arr[3];

	cout << tmp_v1.str() << tmp_v2.str() << tmp_v3.str()
		<< tmp_v4.str() << tmp_v5.str() << tmp_arr.str();

	Json tmp_o; //map<string, Json>

	tmp_o["bool"] = true;
	tmp_o["int"] = 128;
	tmp_o["float"] = 128.33;
	tmp_o["double"] = 256.99;
	tmp_o["str"] = "this is string";
	tmp_o["array"] = tmp_arr;
	cout << tmp_o.str();

	tmp_v1 = tmp_arr;
	cout << "------json_null to json_array------\n" << tmp_v1.str();

	tmp_v2 = tmp_arr;
	cout << "------json_bool to json_array------\n" << tmp_v2.str();

	tmp_v5 = tmp_o;
	cout << "------json_string to json_object]\n" << tmp_v5.str();

	tmp_o = tmp_v2; //v2此处是json_array类型
	cout << "------json_object to json_array------\n" << tmp_o.str();
	
	tmp_arr = tmp_v5; //v5此处是json_object类型
	cout << "------json_array to json_object------\n" << tmp_arr.str();

	Json tmp_arr1;
	tmp_arr1[0] = true;
	tmp_arr1[1] = 123;

	tmp_arr1.append(128.33);
	tmp_arr1.append("nice day~");

	Json tmp_arr2;
	tmp_arr2[0] = true;
	tmp_arr2[1] = 123;

	tmp_arr2.append(128.33);
	tmp_arr2.append("not a nice day~");

	Json tmp_o1; //map<string, Json>
	Json tmp_o2; //map<string, Json>
	tmp_o1["bool"] = true;
	tmp_o1["int"] = 128;
	tmp_o1["float"] = 128.33;
	tmp_o1["double"] = 256.99;
	tmp_o1["str"] = "this is string";
	tmp_o1["array"] = tmp_arr1;
	tmp_o2["bool"] = true;
	tmp_o2["int"] = 128;
	tmp_o2["float"] = 128.33;
	tmp_o2["double"] = 256.99;
	tmp_o2["str"] = "this is string";
	tmp_o2["array"] = tmp_arr1;

	cout << tmp_o2.str();
	tmp_o2.append("append", tmp_v2);
	tmp_o2.append("append2", "hello!");
	cout << tmp_o2.str();

	//判断相等, 预期结果 1 0 0 1 0 1 0
	cout << (tmp_v5 == tmp_arr) << " " << (tmp_o != tmp_v2) << " "
		<< (tmp_v2 == tmp_v5) << " " << (tmp_o1 == tmp_o2) << " "
		<< (tmp_arr1 == tmp_arr2) << " " << (tmp_o1 == tmp_o2) << " "
		<< (tmp_arr1 == tmp_arr2) << endl;

	cout << "tmp_o1: " << tmp_o1.str();
	tmp_o1.remove("int");
	cout << "tmp_o1: " << tmp_o1.str();

	cout << "tmp_arr1: " << tmp_arr1.str();
	tmp_arr1.remove(0);
	cout << "tmp_arr1: " << tmp_arr1.str();


	return 0;
}