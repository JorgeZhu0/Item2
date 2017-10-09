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
	void StuAssignment(Json::Value &root, int index);//将json数据读到Student类中
	void TimeMatch(Department*dep,bool check);//进行时间匹配
	bool UnluckyStudents();//判断一个学生是否一个部门都没被分配到
	void FreetimeDelete(Department&dep);//对已分配到部门的学生删除相应的free_time
	void Redistribute(Department&dep,Department*deps);//对被踢出的学生进行重新分配
	friend int GetIndex(Student*stu, Department*dep, std::string s);//由部门号或者学生号获取索引
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
	void DepAssignment(Json::Value &root, int index);//将json数据读到Student类中
	Json::Value RetMember();//返回Json::Value格式的部门成员以及部门号
	bool OverLimit();//判断人数是否超过限制
	void InterestFilter(Student*stu,Department*dep);//执行兴趣筛选
	bool DeppartmentEmpty();//判断部门人数是否为空
	friend int GetIndex(Student*stu, Department*dep, std::string s);//由部门号或者学生号获取索引
};
bool CompareTime(std::string dep_time, std::string stu_time);//对两个string格式的时间进行比较
int GetIndex(Student*stu, Department*dep, std::string s);//由部门号或者学生号获取索引
std::string TimeCorrection(std::string s);//进行时间修正
#endif
