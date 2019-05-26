/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/



#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "QTool.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
#include<ctime>
#include <regex>
#include <string>
#include <regex>
#include <map>
#include<sys/timeb.h>

using namespace std;

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;


//------------------------------------------

class AliasList
{
public:
	static const string aliasList[65][2];
	static string getName(string alias)
	{
		for (int i = 0; i < 62; i++)
		{
			if (aliasList[i][1] == alias)
			{
				return aliasList[i][0];
			}
		}
		return "";
	}
	static string getAlias(string name)
	{
		for (int i = 0; i < 65; i++)
		{
			if (aliasList[i][0] == name)
			{
				return aliasList[i][1];
			}
		}
		return "";
	}
};
const string AliasList::aliasList[65][2] = { { "����", "sm" }, { "ħ��", "mf" }, { "����", "lz" },{ "����", "ll" }, { "����", "tz" }, { "����", "tx" }, { "����", "mj" }, { "��ò", "wm" }, { "����", "zl" }, { "��־", "yz" }, { "����", "jy" }, { "����", "xy" }, { "���", "kj" }, { "����ѱ��", "dwxy" }, { "����ѧ", "rlx" }, { "����", "gj" }, { "����ѧ", "kgx" }, { "�����빤��", "ysygy" }, { "����", "ps" }, { "�Ȼ�", "mh" }, { "����", "pp" }, { "����ʹ��", "dnsy" }, { "��������", "xypj" }, { "����³��", "kslsh" }, { "����", "bp" }, { "��װ", "qz" }, { "Ǳˮ", "qs" }, { "����", "sb" }, { "�Զ�����ʻ", "zdcjs" }, { "����ѧ", "dzx" }, { "����", "hs" }, { "��", "gd" }, { "���", "sj" }, { "����", "jj" }, { "��ʷ", "ls" }, { "����", "cm" }, { "����", "kh" }, { "��Ծ", "ty" }, { "ĸ��", "my" }, { "����", "fl" }, { "ͼ���ʹ��", "tsgsy" }, { "����", "lt" }, { "����", "sj" }, { "ѧ��", "xw" }, { "��еά��", "jxwx" }, { "ҽѧ", "yx" }, { "��Ȼѧ", "zrx" }, { "�캽", "lh" }, { "����ѧ", "smx" }, { "�������ͻ�е", "czzxjx" }, { "˵��", "sf" }, { "��ʻ", "js" }, { "�������", "xlfx" }, { "����ѧ", "xlx" }, { "����", "dx" }, { "����", "qs" }, { "��ѧ", "kx" }, { "���Ϲ���", "ssgf" }, { "���", "zc" }, { "�����ж�", "ymxd" }, { "����", "sc" }, { "��Ӿ", "yy" }, { "Ͷ��", "tz2" }, { "׷��", "zz" }, { "��Ź", "do" } };

class PPT
{
public:
	int64_t group;
	int64_t qq;
	string propert;
	bool operator<(const PPT& other) const
	{
		if (group < other.group)
			return true;
		else if (group == other.group&&qq < other.qq)
			return true;
		else if (group == other.group&&qq == other.qq&&propert < other.propert)
			return true;
		return false;
	}
	bool operator==(const PPT& other) const
	{
		if (group == other.group&&qq == other.qq&&propert == other.propert)
		{
			return true;
		}
		return false;
	}
	bool operator>(const PPT& other) const
	{
		if (group > other.group)
			return true;
		else if (group == other.group&&qq > other.qq)
			return true;
		else if (group == other.group&&qq == other.qq&&propert > other.propert)
			return true;
		return false;
	}
};

class DataManager
{

private:
	map<PPT, int> properties;
	string dataPath;
public:
	DataManager(){ }
	~DataManager(){}

	void setProperty(int64_t group, int64_t qq, string propert, int data)
	{
		PPT tempPPT = PPT{ group, qq, propert };
		properties[tempPPT] = data;
	}
	int getProperty(int64_t group, int64_t qq, string propert)
	{
		PPT tempPPT = PPT{ group, qq, propert };
		map<PPT, int>::iterator iter;
		iter = properties.find(tempPPT);
		if (iter == properties.end())
		{
			return -1;
		}
		else
		{
			return iter->second;
		}
	}
	bool deleteProperties(int64_t group, int64_t qq = 0, string propert = "")
	{
		map<PPT, int>::iterator iter;
		iter = properties.begin();
		while (iter != properties.end())
		{
#define shouldDel1 (group == iter->first.group)
#define shouldDel2 (qq == 0 || qq == iter->first.qq)
#define shouldDel3 (propert == "" || propert == iter->first.propert)
			if (shouldDel1 && shouldDel2 && shouldDel3)
			{
				properties.erase(iter++);
			}
			else
			{
				iter++;
			}
		}
		return true;
	}
};

class RandomMachine
{
public:

	int get(int limit)
	{
		if (limit < 1)
		{
			limit = 1;
		}
		return rand() % limit + 1;
	}

};

struct Info
{
	int64_t group;
	int64_t qq;
};


//���Ż���set
class OrderManager
{
private:
	DataManager dm;
	RandomMachine rm;
	CQTool cqtool;



	string getName(Info info)
	{
		CQ_Type_GroupMember data;
		cqtool.GetGroupMemberInfo(ac, info.group, info.qq, data);
		return data.nick;
	}
	bool sendGroupMsg(string msg, Info info)
	{
		//������Ϣ
		//cout << "message:\n" << msg << ";group:" << info.group << ";qq:" << info.qq << endl;
		//������Ϣ
		CQ_sendGroupMsg(ac,info.group,msg.data());
		return true;
	}
	bool controller(string orderName, string content, Info info)
	{
		if (orderName == "rc")
			return rc(content, info);
		else if (orderName == "st")
			return st(content, info);
		else if (orderName == "r")
			return d(content, info);
		else if (orderName == "rd")
			return rd(content, info);
		else if (orderName == "san")
			return san(content, info);
		else
			return aliasSystem(orderName, content, info);

	}
	bool rc(string content, Info info)
	{
		string mode = "^([^\\s^0-9]+)\\s*([0-9]{1,3})?$";
		regex reg(mode);
		smatch m;
		regex_match(content, m, reg);
		if (m.str(1) == "")
			return false;
		int data = 0;
		if (m.str(2) != "")
		{
			if (st(m.str(1) + " " + m.str(2), info) != false)
			{
				data = stoi(m.str(2));
			}
			else
			{
				return false;
			}
		}
		else
		{
			data = dm.getProperty(info.group, info.qq, m.str(1));
			if (data == -1)
			{
				return false;
			}
		}

		//�ж�
		string judgeStr;
		int returnRandom = rm.get(100);//�����

		if (returnRandom > data)
		{
			if ((data >= 50 && returnRandom == 100) || (returnRandom >= 96))
				judgeStr = "��ʧ��";
			else judgeStr = "ʧ��";
		}
		else
		{
			if (returnRandom == 1)
			{
				judgeStr = "��ɹ�";
			}
			else if (returnRandom <= data / 5)
			{
				judgeStr = "���ѳɹ�";
			}
			else if (returnRandom <= data / 2)
			{
				judgeStr = "���ѳɹ�";
			}
			else
			{
				judgeStr = "�ɹ�";
			}
		}
		string sendStr = getName(info) + "����\"" + m.str(1) + "\"" + to_string(data) + "�춨: D100=" + to_string(returnRandom) + "/100 " + judgeStr;
		sendGroupMsg(sendStr, info);
		return true;
	}
	bool san(string content, Info info)
	{
		string mode = "^(([0-9])(d([0-9]{1,2}))?)\\s(([0-9])(d([0-9]{1,2}))?)$";
		regex reg(mode);
		smatch m;
		regex_match(content, m, reg);
		if (m.str(2) == "" || m.str(6) == "")
		{
			return false;
		}
		int dataLz = dm.getProperty(info.group, info.qq, "����");//����ֵ
		if (dataLz==-1)
		{
			return false;
		}


		int num1 = stoi(m.str(2));//��һ��������
		int num3 = stoi(m.str(6));//�ڶ���������
		int num2;
		int num4;

		//Ϊ�⼸�����ֳ�ʼ��
		if (m.str(4) == ""){ num2 = 0; }
		else { num2 = stoi(m.str(4)); }
		if (m.str(8) == ""){ num4 = 0; }
		else { num4 = stoi(m.str(8)); }

		int delData=0;//������
		int randomInt = rm.get(100);
		string text = to_string(randomInt) + " ";//��������ʧ���ı�
		string delText = "";//��������3+4+2֮����ı�
		if ((dataLz>=50 && randomInt == 100)||randomInt>=96)
		{
			if (m.str(8) == "")
			{
				delData = num3;
				delText = m.str(6);
			}
			else
			{
				delData = num4;
				delText = m.str(8);
			}
			text += "��ʧ��";
		}
		else if (randomInt > dataLz)
		{
			if (m.str(8) == "")
			{
				delData = num3;
				delText = m.str(6);
			}
			else
			{
				for (int i = 0; i < num3; i++)
				{
					int tempNum = rm.get(num4);
					delData += tempNum;
					if (i>0)delText += "+";
					delText += to_string(tempNum);
				}
				delText = delText + "=" + to_string(delData);
			}
			text += "ʧ��";
		}
		else
		{
			if (m.str(4) == "")
			{
				delData = num1;
				delText = m.str(2);
			}
			else
			{
				for (int i = 0; i < num1; i++)
				{
					int tempNum = rm.get(num2);
					delData += tempNum;
					if (i>0)delText += "+";
					delText += to_string(tempNum);
				}
				delText = delText + "=" + to_string(delData);
			}
			text += "�ɹ�";
		}
		string sendStr = "�춨san check," + text + ",�۳�" + delText + "��\"����\",���" + getName(info) + "\"����\"��ʣ��" + to_string(dataLz - delData);
		dm.setProperty(info.group, info.qq,"����", dataLz-delData);//������������
		sendGroupMsg(sendStr,info);
		return true;
	}
	bool st(string content, Info info)
	{
		string mode = "^([^\\s^0-9]+)\\s*([0-9]{1,3})$";
		regex reg(mode);
		smatch m;
		regex_match(content, m, reg);
		if (m.str(1) != ""&&m.str(2) != "")
		{
			int data = stoi(m.str(2));
			if (data > 0)
			{
				dm.setProperty(info.group, info.qq, m.str(1), data);
				sendGroupMsg(m.str(1) + " ��������Ϊ:" + m.str(2), info);
			}

			else
				return false;
		}
		else
		{
			return false;
		}

		return true;
	}
	bool d(string content, Info info)
	{
		string mode = "^(([0-9])[dD]([0-9]{1,4})(\\+([0-9]))?)?([^+^0-9^a-z]+)?$";
		regex reg(mode);
		smatch m;
		regex_match(content, m, reg);
		int num1, num2;
		if (m.str(1) != "")
		{
			num1 = stoi(m.str(2));
			num2 = stoi(m.str(3));
		}
		else
		{
			num1 = 1;
			num2 = 100;
		}
		if (num1 == 0 || num2 == 0)
		{
			return false;
		}
		string sendStr="";
		if (m.str(6) != "")
		{
			sendStr += "����" + m.str(6) + ",";
		}
		sendStr += getName(info) + "������: " + to_string(num1) + "D" + to_string(num2) + "=";
		if (num1 == 1)
		{
			int tempNum = rm.get(num2);
			sendStr += to_string(tempNum);
			if (m.str(5) != "")
			{
				tempNum += stoi(m.str(5));//ֵ����
				sendStr = sendStr + m.str(4) + "=" + to_string(tempNum);
			}
			
		}
		else if (num1<10)
		{
			int count = 0;
			for (int i = 0; i < num1; i++)
			{
				int tempNum = rm.get(num2);
				count += tempNum;
				if (i>0)
				{
					sendStr += "+";
				}
				sendStr += to_string(tempNum);
			}
			if (m.str(5)!="")
			{
				count += stoi(m.str(5));
				sendStr += m.str(4);
			}
			sendStr = sendStr + "=" + to_string(count);
		}
		sendGroupMsg(sendStr, info);
		return true;
	}
	bool rd(string content, Info info)
	{
		if (content == "")
		{
			return d("1d100", info);
		}
		else
		{
			return d("1d" + content, info);
		}
	}
	bool aliasSystem(string aliasName, string content, Info info)
	{
		string name = AliasList::getName(aliasName);
		if (name != "")
		{
			return rc(name + " " + content, info);
		}
		return false;
	}
public:
	bool order(const char* orderStr, int64_t fromGroup, int64_t fromQQ)
	{
		getName(Info{ fromGroup, fromQQ });
		timeb stb;
		ftime(&stb);
		srand((unsigned)stb.millitm);
		if (orderStr == NULL)
		{
			return false;
		}
		string mode = "^\\.([a-z]+)\\s*(.*)$";
		//sendGroupMsg(orderStr, Info{ fromGroup, fromQQ });
		//return true;
		regex reg(mode);
		smatch m;
		string str = orderStr;
		regex_match(str, m, reg);
		if (m.str(1) == "")
		{
			return false;
		}
		else
		{
			if (controller(m.str(1), m.str(2), Info{ fromGroup, fromQQ })==false)
			{
				sendGroupMsg("�����������!", Info{ fromGroup, fromQQ });
			}
		}
		return true;
	}

};
//------------------------------------------
OrderManager *om;



/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {
	if (om != NULL)
	{
		delete om;
		om = NULL;
	}
	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	om = new OrderManager();
	enabled = true;
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	delete om;
	om = NULL;
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	return EVENT_IGNORE;
}


/*
* Type=2 Ⱥ��Ϣ
*/

CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	

	//��ֹ����Ⱥ�ļ������Ϣ
	if (strcmp(msg, "") == 0)
	{
		return EVENT_IGNORE;
	}
	if (om->order(msg, fromGroup, fromQQ))
	{
		return EVENT_BLOCK;
	}
	
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "����menuB�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}
