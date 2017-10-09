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
string depId[30]; // ��¼���ű��
map<string, bool>id; // ������Ƿ��ظ�
bool flag[7][20]; //��¼ÿ��ʱ���
bool vis[30]; // ��¼��ʹ�õĲ��ű��
bool used[30]; // ��¼��ʹ�õı�ǩ

// �������Ψһ�Ĳ��ű�ţ�D��ͷ�����3λ���֣���D001
string getDepId() {
	string DepId = "D";
	int count = 0;
	do {
		if (count >= 1)// ��ֹ�����ظ�DepId������
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

// �������Ψһ��ѧ��ѧ�ţ�03150��ͷ����4λ�������1-9���֣���031502626
string getStuId() {
	string StuId = "03150";
	int count = 0;
	do
	{
		if (count >= 1)// ��ֹ�����ظ�StuId������
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

// �������ʱ��
string getTime() {
	int d, h1;
	do {
		d = rand() % 7;
		h1 = rand() % 13;
	} while (flag[d][h1] || flag[d][h1 + 1] || flag[d][h1 + 2]);
	string res = day[d] + "." + hour[h1] + ":00" + "~" + hour[h1 + 2] + ":00";
	flag[d][h1] = flag[d][h1 + 1] = flag[d][h1 + 2] = true;// ��ֹʱ���ͻ
	return res;
}

// ����
void data_dep(int n) {
	id.clear(); 
	Json::Value Item;
	int tgn, i, j;

	for (i = 0; i < n; ++i) {
		memset(used, false, sizeof(used));
		memset(flag, false, sizeof(flag));

		// ���ɳ���ʱ�� 
		int cnt = rand() % 4 + 2;
		for (j = 0; j < cnt; ++j) {
			string tmp;
			tmp = getTime();
			Item["event_schedules"].append(tmp);
		}

		// ���ɲ��ű��
		Item["department_no"] = depId[i] = getDepId();

		// ���ɲ�����������
		Item["member_limit"] = rand() % 5 + 10;

		// ���ɱ�ǩ
		tgn = rand() % 4 + 2;
		for (j = 0; j < tgn; ++j) {
			int inx;
			do {
				inx = rand() % 12;
			} while (used[inx]);
			used[inx] = true;
			// ��������ǩ������ʹ�ù�
			vis[inx] = true;
			Item["tags"].append(tag[inx]);
		}
		MAIN["departments"].append(Item);
		Item.clear();
	}
	return;
}

// ѧ��
void data_stu(int n) {
	Json::Value Item;
	int i, j;
	id.clear();
	bool in[10];
	for (i = 0; i < n; ++i) {
		memset(used, false, sizeof(used));
		memset(flag, false, sizeof(flag));
		memset(in, false, sizeof(in));

		// �������ѧ�����
		Item["student_no"] = getStuId();
		// ������ɿ���ʱ��
		int cnt = rand() % 10 + 7;
		for (j = 0; j < cnt; ++j) {
			string tmp;
			tmp = getTime();
			Item["free_time"].append(tmp);
		}

		// ���Ѵ����Ĳ�����������<=5������ 
		int deps = rand() % 6;
		for (j = 0; j < deps; ++j) {
			int inx;
			do {
				inx = rand() % 20;
			} while (in[inx]);
			Item["applications_department"].append(depId[inx]);
			in[inx] = true;
		}

		// ������Ѿ���������ʹ�ù��ı�ǩ�г��tgn����ǩ
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