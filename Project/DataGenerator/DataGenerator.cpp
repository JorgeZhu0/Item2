#include<iostream>
#include"json.h"
#include<stdlib.h>
#include<cstring>
#include<string>
#include<cstdio>
#include<algorithm>
#include<map>
#include<fstream>
using namespace std;
Json::Value MAIN; 

string tag[20] = {"Social","Football","Basketball","Swimming","Running","Chess","Dancing","Sing","Jump","Cool","Beauty","Camera"};
string day[7] = { "Mon", "Tues", "Wed", "Thur","Fri","Sat", "Sun"}; 
string hour[20] = { "08", "09","10","11","12","13", "14", "15", "16","17", "18","19", "20","21","22" };
string depId[30]; // 记录部门编号
map<string, bool>id; // 检查编号是否重复
bool flag[7][20]; //记录每天时间段
bool vis[30]; // 记录被使用的部门编号
bool used[30]; // 记录被使用的标签

// 随机生成唯一的部门编号：D开头，后加3位数字，如D001
string getDepId() {
	string DepId = "D";
	int count = 0;
	do {
		if (count >= 1)// 防止生成重复DepId，重置
		{
			DepId = "D";
		}
		for (int i = 0; i < 3; i++) {
			DepId += rand() % 10 + '0';
		}
	} while (id[DepId]);
	id[DepId] = true;
	return DepId;
}

// 随机生成唯一的学生学号：03150开头，后4位随机生成1-9数字，如031502626
string getStuId() {
	string StuId = "03150";
	int count = 0;
	do
	{
		if (count >= 1)// 防止生成重复StuId，重置
		{
			StuId = "03150";
		}
		for (int i = 0; i < 4; i++) {
			StuId += rand() % 10 + '0';
		}
		count++;
	} while (id[StuId]);
	id[StuId] = true;
	return StuId;
}

// 随机生成时间
string getTime() {
	int d, h1;
	do {
		d = rand() % 7;
		h1 = rand() % 13;
	} while (flag[d][h1] || flag[d][h1 + 1] || flag[d][h1 + 2]);
	string res = day[d] + "." + hour[h1] + ":00" + "~" + hour[h1 + 2] + ":00";
	flag[d][h1] = flag[d][h1 + 1] = flag[d][h1 + 2] = true;// 防止时间冲突
	return res;
}

// 部门
void data_dep(int n) {
	id.clear(); 
	Json::Value Item;
	int tgn, i, j;

	for (i = 0; i < n; ++i) {
		memset(used, false, sizeof(used));
		memset(flag, false, sizeof(flag));

		// 生成常规活动时间 
		int cnt = rand() % 4 + 2;
		for (j = 0; j < cnt; ++j) {
			string tmp;
			tmp = getTime();
			Item["event_schedules"].append(tmp);
		}

		// 生成部门编号
		Item["department_no"] = depId[i] = getDepId();

		// 生成部门纳新人数
		Item["member_limit"] = rand() % 5 + 10;

		// 生成标签
		tgn = rand() % 4 + 2;
		for (j = 0; j < tgn; ++j) {
			int inx;
			do {
				inx = rand() % 12;
			} while (used[inx]);
			used[inx] = true;
			// 标记这个标签被部门使用过
			vis[inx] = true;
			Item["tags"].append(tag[inx]);
		}
		MAIN["departments"].append(Item);
		Item.clear();
	}
	return;
}

// 学生
void data_stu(int n) {
	Json::Value Item;
	int i, j;
	id.clear();
	bool in[10];
	for (i = 0; i < n; ++i) {
		memset(used, false, sizeof(used));
		memset(flag, false, sizeof(flag));
		memset(in, false, sizeof(in));

		// 随机生成学生编号
		Item["student_no"] = getStuId();
		// 随机生成空余时间
		int cnt = rand() % 10 + 7;
		for (j = 0; j < cnt; ++j) {
			string tmp;
			tmp = getTime();
			Item["free_time"].append(tmp);
		}

		// 从已创建的部门中随机抽出<=5个部门 
		int deps = rand() % 6;
		for (j = 0; j < deps; ++j) {
			int inx;
			do {
				inx = rand() % 20;
			} while (in[inx]);
			Item["applications_department"].append(depId[inx]);
			in[inx] = true;
		}

		// 随机从已经被部门所使用过的标签中抽出tgn个标签
		int tgn = rand() % 4 + 2;
		for (j = 0; j < tgn; ++j) {
			int inx;
			do {
				inx = rand() % 12;
			} while (used[inx] || !vis[inx]);
			used[inx] = true;
			Item["tags"].append(tag[inx]);
		}

		MAIN["students"].append(Item);
		Item.clear();
	}
	return;
}


int main() {
	data_dep(20);
	data_stu(300);
	Json::StyledWriter writer;
	string str = writer.write(MAIN);
	std::ofstream out;
	out.open("input_data.txt");
	out << str;
	out.close();
	system("pause");
}