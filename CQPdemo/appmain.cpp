/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/



#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "QTool.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include<ctime>
#include <regex>
#include <string>
#include <regex>
#include <map>
#include<sys/timeb.h>

using namespace std;

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
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
const string AliasList::aliasList[65][2] = { { "生命", "sm" }, { "魔法", "mf" }, { "理智", "lz" },{ "力量", "ll" }, { "体质", "tz" }, { "体型", "tx" }, { "敏捷", "mj" }, { "外貌", "wm" }, { "智力", "zl" }, { "意志", "yz" }, { "教育", "jy" }, { "幸运", "xy" }, { "会计", "kj" }, { "动物驯养", "dwxy" }, { "人类学", "rlx" }, { "估价", "gj" }, { "考古学", "kgx" }, { "艺术与工艺", "ysygy" }, { "炮术", "ps" }, { "魅惑", "mh" }, { "攀爬", "pp" }, { "电脑使用", "dnsy" }, { "信用评级", "xypj" }, { "克苏鲁神话", "kslsh" }, { "爆破", "bp" }, { "乔装", "qz" }, { "潜水", "qs" }, { "闪避", "sb" }, { "自动车驾驶", "zdcjs" }, { "电子学", "dzx" }, { "话术", "hs" }, { "格斗", "gd" }, { "射击", "sj" }, { "急救", "jj" }, { "历史", "ls" }, { "催眠", "cm" }, { "恐吓", "kh" }, { "跳跃", "ty" }, { "母语", "my" }, { "法律", "fl" }, { "图书馆使用", "tsgsy" }, { "聆听", "lt" }, { "锁匠", "sj" }, { "学问", "xw" }, { "机械维修", "jxwx" }, { "医学", "yx" }, { "自然学", "zrx" }, { "领航", "lh" }, { "神秘学", "smx" }, { "操作重型机械", "czzxjx" }, { "说服", "sf" }, { "驾驶", "js" }, { "心理分析", "xlfx" }, { "心理学", "xlx" }, { "读唇", "dx" }, { "骑术", "qs" }, { "科学", "kx" }, { "手上功夫", "ssgf" }, { "侦查", "zc" }, { "隐秘行动", "ymxd" }, { "生存", "sc" }, { "游泳", "yy" }, { "投掷", "tz2" }, { "追踪", "zz" }, { "斗殴", "do" } };

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


//待优化成set
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
		//测试信息
		//cout << "message:\n" << msg << ";group:" << info.group << ";qq:" << info.qq << endl;
		//发送信息
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

		//判定
		string judgeStr;
		int returnRandom = rm.get(100);//随机数

		if (returnRandom > data)
		{
			if ((data >= 50 && returnRandom == 100) || (returnRandom >= 96))
				judgeStr = "大失败";
			else judgeStr = "失败";
		}
		else
		{
			if (returnRandom == 1)
			{
				judgeStr = "大成功";
			}
			else if (returnRandom <= data / 5)
			{
				judgeStr = "极难成功";
			}
			else if (returnRandom <= data / 2)
			{
				judgeStr = "困难成功";
			}
			else
			{
				judgeStr = "成功";
			}
		}
		string sendStr = getName(info) + "进行\"" + m.str(1) + "\"" + to_string(data) + "检定: D100=" + to_string(returnRandom) + "/100 " + judgeStr;
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
		int dataLz = dm.getProperty(info.group, info.qq, "理智");//理智值
		if (dataLz==-1)
		{
			return false;
		}


		int num1 = stoi(m.str(2));//第一个骰骰子
		int num3 = stoi(m.str(6));//第二个骰骰子
		int num2;
		int num4;

		//为这几个数字初始化
		if (m.str(4) == ""){ num2 = 0; }
		else { num2 = stoi(m.str(4)); }
		if (m.str(8) == ""){ num4 = 0; }
		else { num4 = stoi(m.str(8)); }

		int delData=0;//新数据
		int randomInt = rm.get(100);
		string text = to_string(randomInt) + " ";//用来描述失败文本
		string delText = "";//用来描述3+4+2之类的文本
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
			text += "大失败";
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
			text += "失败";
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
			text += "成功";
		}
		string sendStr = "检定san check," + text + ",扣除" + delText + "点\"理智\",如今" + getName(info) + "\"理智\"还剩下" + to_string(dataLz - delData);
		dm.setProperty(info.group, info.qq,"理智", dataLz-delData);//重新设置数据
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
				sendGroupMsg(m.str(1) + " 属性设置为:" + m.str(2), info);
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
			sendStr += "由于" + m.str(6) + ",";
		}
		sendStr += getName(info) + "骰出了: " + to_string(num1) + "D" + to_string(num2) + "=";
		if (num1 == 1)
		{
			int tempNum = rm.get(num2);
			sendStr += to_string(tempNum);
			if (m.str(5) != "")
			{
				tempNum += stoi(m.str(5));//值增加
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
				sendGroupMsg("输入命令错误!", Info{ fromGroup, fromQQ });
			}
		}
		return true;
	}

};
//------------------------------------------
OrderManager *om;



/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
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
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	om = new OrderManager();
	enabled = true;
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	delete om;
	om = NULL;
	enabled = false;
	return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	return EVENT_IGNORE;
}


/*
* Type=2 群消息
*/

CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	

	//防止出现群文件后空信息
	if (strcmp(msg, "") == 0)
	{
		return EVENT_IGNORE;
	}
	if (om->order(msg, fromGroup, fromQQ))
	{
		return EVENT_BLOCK;
	}
	
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "这是menuA，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "这是menuB，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}
