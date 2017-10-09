#ifndef HEADER_H_
#define HEADER_H_
#include<string>
#include<vector>
#include"json.h"
class Student
{
private:
	std::string student_no;
	std::vector<std::string>free_time;
	int freetimes;
	std::string *applications_department;
	int applications;
	std::string tags;
	std::vector<std::string>department_no;
	friend class Department;
public:
	Student();
	~Student();
	void StuAssignment(Json::Value &root, int index);//��json���ݶ���Student����
	void TimeMatch(Department*dep,bool check);//����ʱ��ƥ��
	bool UnluckyStudents();//�ж�һ��ѧ���Ƿ�һ�����Ŷ�û�����䵽
	void FreetimeDelete(Department&dep);//���ѷ��䵽���ŵ�ѧ��ɾ����Ӧ��free_time
	void Redistribute(Department&dep,Department*deps);//�Ա��߳���ѧ���������·���
	friend int GetIndex(Student*stu, Department*dep, std::string s);//�ɲ��źŻ���ѧ���Ż�ȡ����
};
class Department
{
private:
	std::string department_no;
	int member_limit;
	std::string tags;
	std::string *event_schedules;
	int schedules;
	std::vector<std::string>member;
	friend class Student;
public:
	Department();
	~Department();
	void DepAssignment(Json::Value &root, int index);//��json���ݶ���Student����
	Json::Value RetMember();//����Json::Value��ʽ�Ĳ��ų�Ա�Լ����ź�
	bool OverLimit();//�ж������Ƿ񳬹�����
	void InterestFilter(Student*stu,Department*dep);//ִ����Ȥɸѡ
	bool DeppartmentEmpty();//�жϲ��������Ƿ�Ϊ��
	friend int GetIndex(Student*stu, Department*dep, std::string s);//�ɲ��źŻ���ѧ���Ż�ȡ����
};
bool CompareTime(std::string dep_time, std::string stu_time);//������string��ʽ��ʱ����бȽ�
int GetIndex(Student*stu, Department*dep, std::string s);//�ɲ��źŻ���ѧ���Ż�ȡ����
std::string TimeCorrection(std::string s);//����ʱ������
#endif
