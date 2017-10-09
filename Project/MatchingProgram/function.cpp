#include"header.h"
using namespace std;
Student::Student()
{
	applications_department = NULL;
}
Student::~Student()
{
	if (applications_department != NULL)delete[]applications_department;
}
void Student::StuAssignment(Json::Value &root, int index)//��Json���ݶ��뵽Student��
{
	int i;
	int freetime_count = root["students"][(Json::Value::UInt)index]["free_time"].size();
	freetimes = freetime_count;
	for (i = 0; i < freetime_count; i++)
	{
		free_time.push_back(TimeCorrection(root["students"][(Json::Value::UInt)index]["free_time"][(Json::Value::UInt)i].asString()));
	}

	student_no = root["students"][(Json::Value::UInt)index]["student_no"].asString();

	int application_count= root["students"][(Json::Value::UInt)index]["applications_department"].size();
	applications_department = new string[application_count];
	applications = application_count;
	for (i = 0; i < application_count; i++)
	{
		applications_department[i]= root["students"][(Json::Value::UInt)index]["applications_department"][(Json::Value::UInt)i].asString();
	}

	int tags_count = root["students"][(Json::Value::UInt)index]["tags"].size();
	for (i = 0; i < tags_count; i++)
	{
		tags+= root["students"][(Json::Value::UInt)index]["tags"][(Json::Value::UInt)i].asString();
	}
}
Department::Department()
{
	event_schedules = NULL;
}
Department::~Department()
{
	if (event_schedules != NULL)delete[]event_schedules;
}
void Department::DepAssignment(Json::Value &root, int index)//��Json���ݶ��뵽Department��
{
	int i;
	int schedules_count = root["departments"][(Json::Value::UInt)index]["event_schedules"].size();
	event_schedules = new string[schedules_count];
	schedules = schedules_count;
	for (i = 0; i < schedules_count; i++)
	{
		event_schedules[i]= TimeCorrection(root["departments"][(Json::Value::UInt)index]["event_schedules"][(Json::Value::UInt)i].asString());
	}

	member_limit = root["departments"][(Json::Value::UInt)index]["member_limit"].asInt();

	department_no = root["departments"][(Json::Value::UInt)index]["department_no"].asString();

	int tags_count = root["departments"][(Json::Value::UInt)index]["tags"].size();
	for (i = 0; i < tags_count; i++)
	{
		tags += root["departments"][(Json::Value::UInt)index]["tags"][(Json::Value::UInt)i].asString();
		tags += ".";
	}
}
void Student::TimeMatch(Department*dep,bool check)//ֻҪ����ʱ����־Ը���ŵ�һ���ʱ��ƥ��ͷ���
{
	int app_count,schedule_count,freetime_count, number;
	bool flag;
	for (app_count = 0; app_count < applications; app_count++)
	{
		number = GetIndex(NULL, dep, applications_department[app_count]);
		if (check == false || dep[number].member.size() < dep[number].member_limit)
		{
			for (schedule_count = 0; schedule_count < dep[number].schedules; schedule_count++)
			{
				for (freetime_count = 0, flag = false; freetime_count < freetimes; freetime_count++)
				{
					if (CompareTime(dep[number].event_schedules[schedule_count], *(free_time.begin() + freetime_count)))
					{
						flag = true;
						break;
					}
				}
				if (flag == true)break;
			}
			if (flag == true)
			{
				department_no.push_back(applications_department[app_count]);
				dep[number].member.push_back(student_no);
				FreetimeDelete(dep[number]);
			}
		}
	}
}
bool CompareTime(string dep_time, string stu_time)//���ַ�����ʽ��ʱ����бȽϣ������Żʱ����ѧ������ʱ�䷶Χ�ڷ���true
{
	int dep_pos, stu_pos;
	if (dep_time.compare(0, 3, stu_time, 0, 3) == 0)
	{
		dep_pos = dep_time.find(".", 0);
		stu_pos = stu_time.find(".", 0);
		if (dep_time.compare(dep_pos, 6, stu_time, stu_pos, 6) >= 0)
		{
			dep_pos = dep_time.find("~", 0);
			stu_pos = stu_time.find("~", 0);
			if (dep_time.compare(dep_pos, 6, stu_time, stu_pos, 6) <= 0)
			{
				return true;
			}
		}
	}
	return false;
}
bool Department::OverLimit()//�жϵ�ǰ���������Ƿ񳬹�����
{
	if (member.size() > member_limit)return true;
	else
		return false;
}
void Department::InterestFilter(Student*stu,Department*dep)//�Գ������������Ĳ���ִ����Ȥɸѡ�ĺ���
{
	int filter_number = member.size() - member_limit;
	int pos1, pos2,flag;
	string interest;
	int i;
	vector<string>::iterator p2;
	int p1;
	for (p1 = 0; (member.begin()+p1) != member.end();p1++)//ѡ��һ�����ų�Ա
	{
		i = GetIndex(stu, NULL, *(member.begin() + p1));//�õ��ò��ų�Ա������
		for (pos1 = 0; pos1 < tags.size();)//�鿴�ó�Ա����Ȥ��ǩ�벿�ű�ǩ�Ƿ���ƥ����
		{
			pos2 = tags.find(".", pos1);
			interest = tags.substr(pos1, pos2 - pos1);
			pos1 = pos2 + 1;
			if (stu[i].tags.find(interest, 0) != string::npos)
			{
				flag = true;
				break;
			}
			flag = false;
		}
		if (flag == false)//û��ƥ����޳��ó�Ա
		{
			member.erase(member.begin() + p1, member.begin() + p1+1);
			for (p2 = stu[i].department_no.begin(); p2 != stu[i].department_no.end(); p2++)
			{
				if (department_no == *p2)
				{
					stu[i].department_no.erase(p2, p2 + 1);
					stu[i].Redistribute(*this, dep);
					break;
				}
			}
			filter_number--;
			p1--;
		}
		if (filter_number == 0)break;//����޳��ĳ�Ա�����ˣ���ֹͣ
	}
	while (filter_number != 0)//���Գ�������������������޳�
	{
		i = GetIndex(stu, NULL, *(member.end() - 1));
		member.erase(member.end() - 1, member.end());
		for (p2 = stu[i].department_no.begin(); p2 != stu[i].department_no.end(); p2++)
		{
			if (department_no == *p2)
			{
				stu[i].department_no.erase(p2, p2 + 1);
				stu[i].Redistribute(*this, dep);
				break;
			}
		}
		filter_number--;
	}
}
int GetIndex(Student*stu, Department*dep, string s)//���ѧ�Ż��źŵ���������
{
	int i=0;
	if (stu==NULL)
	{
		for (i = 0; i < 20; i++)
		{
			if (dep[i].department_no == s)break;
		}
	}
	else
	{
		for (i = 0; i < 300; i++)
		{
			if (stu[i].student_no == s)break;
		}
	}
	return i;
}
bool Student::UnluckyStudents()//�ж���Щ��δ���䵽���ŵ�ѧ��
{
	if (department_no.empty())return true;
	else
		return false;
}
Json::Value Department::RetMember()//����Json::Value��ʽ�Ĳ��ų�Ա�Լ����ź�
{
	Json::Value val;
	vector<string>::iterator p1;
	for (p1 = member.begin(); p1 != member.end(); p1++)
	{
		val["member"].append(Json::Value(*p1));
	}
	val["department_no"] = Json::Value(department_no);
	return val;
}
bool Department::DeppartmentEmpty()//�ж��ĸ�����û�з��䵽ѧ��
{
	if (member.empty())return true;
	else
		return false;
}
std::string TimeCorrection(std::string s)//�Բ��淶ʱ�����У���ĺ��������8:00����Ϊ08:00
{
	int j = s.find(":", 0);
	if (s[j - 2] == '.')
	{
		s.insert(j - 1, "0");
		j = s.find(":", 8);
		if (s[j - 2] == '~')
		{
			s.insert(j - 1, "0");
		}
	}
	return s;
}
void Student::FreetimeDelete(Department&dep)//�����Żʱ����Ѽ���ò��ŵ�ѧ��free_time�п۳�
{
	string s,s1, s2;
	int pos1,pos2;
	int schedule_count, freetime_count;
	for (schedule_count = 0; schedule_count < dep.schedules; schedule_count++)
	{
		for (freetime_count = 0; freetime_count < freetimes; freetime_count++)
		{
			if (CompareTime(dep.event_schedules[schedule_count], *(free_time.begin() + freetime_count)))
			{
				s1 = *(free_time.begin() + freetime_count);
				s2 = dep.event_schedules[schedule_count];
				free_time.erase(free_time.begin() + freetime_count, free_time.begin() + freetime_count + 1);
				freetimes--;
				pos1 = s1.find("~", 8);
				if (s1.compare(0, 9, s2, 0, 9)<0)
				{
					s = s1;
					pos2 = s2.find(".", 0);
					s.replace(pos1 + 1, 5, s2, pos2 + 1, 5);
					free_time.push_back(s);
					freetimes++;
				}
				if (s1.compare(pos1 + 1, 5, s2, pos1 + 1, 5) > 0)
				{
					s = s1;
					pos2 = s1.find(".", 0);
					s.replace(pos2 + 1, 5, s2, pos1 + 1, 5);
					free_time.push_back(s);
					freetimes++;
				}
				break;
			}
		}
	}
}
void Student::Redistribute(Department&dep,Department*deps)//�Ա��߳���ѧ���������·���
{
	int schedule_count, freetime_count;
	string s,s1, s2;
	int pos1, pos2;
	for (schedule_count = 0; schedule_count < dep.schedules; schedule_count++)
	{
		s2 = dep.event_schedules[schedule_count];
		for (freetime_count = 0,s=s2; freetime_count < freetimes; freetime_count++)
		{
			s1 = *(free_time.begin() + freetime_count);
			if (s1.compare(0, 3, s2, 0, 3) == 0)
			{
				pos1 = s1.find("~", 8);
				pos2 = s2.find(".", 0);
				if (s1.compare(pos1 + 1, 5, s2, pos2 + 1, 5) == 0)
				{
					s1.replace(pos1 + 1, 5, s2, pos1 + 1, 5);
					s= s1;
					free_time.erase(free_time.begin() + freetime_count, free_time.begin() + freetime_count + 1);
					freetimes--;
					break;
				}
			}
		}
		for (freetime_count = 0; freetime_count < freetimes; freetime_count++)
		{
			s1 = *(free_time.begin() + freetime_count);
			if (s1.compare(0, 3, s2, 0, 3) == 0)
			{
				pos1 = s1.find(".", 0);
				pos2 = s2.find("~", 8);
				if (s1.compare(pos1 + 1, 5, s2, pos2 + 1, 5) == 0)
				{
					s.replace(pos2 + 1, 5, s1, pos2 + 1, 5);
					free_time.erase(free_time.begin() + freetime_count, free_time.begin() + freetime_count + 1);
					freetimes--;
					break;
				}
			}
		}
		free_time.push_back(s);
		freetimes++;
	}
	TimeMatch(deps, true);
}