#include"header.h"
#include <iostream>
#include<cstring>
#include<fstream>
using namespace std;

int main()
{
	ifstream fcin;
	ofstream fcout;
	fcin.open(".\\input_data.txt");
	Json::Value root;
	Json::Reader jsonreader;
	Json::Value val;
	if (!jsonreader.parse(fcin, root))
	{
		return -1;
	}
	fcin.close();
	int i;
	Student stu[300];
	Department dep[20];
	vector<string>unlucky_student;
	vector<string>::iterator p1;
	for (i = 0; i < 300; i++)
	{
		stu[i].StuAssignment(root, i);
	}

	for (i = 0; i < 20; i++)
	{
		dep[i].DepAssignment(root, i);
	}

	for (i = 0; i < 300; i++)
	{
		stu[i].TimeMatch(dep,false);
	}

	for (i = 0; i < 20; i++)
	{
		if (dep[i].OverLimit() == true)dep[i].InterestFilter(stu,dep);
	}

	for (i = 0; i < 300; i++)
	{
		if (stu[i].UnluckyStudents())unlucky_student.push_back(root["students"][i]["student_no"].asString());
	}
	val["unlucky_student"].resize(0);
	val["admitted"].resize(0);
	val["unlucky_department"].resize(0);
	for (p1 = unlucky_student.begin(); p1<unlucky_student.end(); p1++)
	{
		val["unlucky_student"].append(Json::Value(*p1));
	}
	for (i = 0; i < 20; i++)
	{
		if (dep[i].DeppartmentEmpty() == false)
		{
			val["admitted"].append(Json::Value(dep[i].RetMember()));
		}
		else
		{
			val["unlucky_department"].append(Json::Value(root["departments"][(Json::Value::UInt)i]["department_no"].asString()));
		}
	}
	Json::StyledWriter sw;
	std::string s1 = sw.write(val);
	fcout.open(".\\output_data.txt");
	fcout << s1;
	fcout.close();
}