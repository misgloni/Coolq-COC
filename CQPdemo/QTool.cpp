#include "stdafx.h"
#include "QTool.h"
#include "cqp.h"
#include "base64.h"
#include <math.h>

CQTool::CQTool()
{
}


CQTool::~CQTool()
{
}

bool CQTool::GetGroupMemberInfo(int auth, int64_t groupID, int64_t qqid, CQ_Type_GroupMember& groupMember)
{
	std::string encodeStr = CQ_getGroupMemberInfoV2(auth, groupID, qqid, false);
	if (0 == encodeStr.size())
	{
		return false;
	}
	std::string decodeStr = base64_decode(encodeStr);
	if (decodeStr.size() < 40)
	{
		CQ_addLog(auth, CQLOG_ERROR, "CQTool::GetGroupMemberInfo", "decodeStr.size() < 40");
		return false;
	}
	return CQ_Tools_TextToGroupMember(decodeStr, groupMember);
}

bool CQTool::CQ_Tools_TextToGroupMember(std::string decodeStr, CQ_Type_GroupMember& groupMember)
{
	int i = 0;
	int count = 8;
	groupMember.GroupID = CoolQ_Tools_GetNum(i, count, decodeStr);
	i += count;

	count = 8;
	groupMember.QQID = CoolQ_Tools_GetNum(i, count, decodeStr);
	i += count;

	groupMember.username = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.username.size(); //2 means string length bit

	groupMember.nick = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.nick.size();
	
	count = 4;
	groupMember.sex = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;
	
	count = 4;
	groupMember.age = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;
	
	groupMember.area = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.area.size();
	
	count = 4;
	groupMember.jointime = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;
	
	count = 4;
	groupMember.lastsent = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;
	
	groupMember.level_name = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.level_name.size();
	
	count = 4;
	groupMember.permission = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;
	
	count = 4;
	groupMember.unfriendly = (CoolQ_Tools_GetNum(i, 4, decodeStr) == 1);
	i += count;
	
	groupMember.title = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.title.size();
	
	count = 4;
	groupMember.titleExpiretime = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;
	
	count = 4;
	groupMember.nickcanchange = (CoolQ_Tools_GetNum(i, 4, decodeStr) == 1);
	i += count;
	
	return true;
}

int64_t CQTool::CoolQ_Tools_GetNum(int index, long length, const std::string &decodeStr)
{
	std::string subString = decodeStr.substr(index, length);
	return From256to10(subString);
}

std::string CQTool::CoolQ_Tools_GetStr(int index, const std::string &decodeStr)
{
	int length = (int)CoolQ_Tools_GetNum(index, 2, decodeStr);
	if (length == 0)
	{
		return "";
	}

	return decodeStr.substr(index+2, length);
}


int64_t CQTool::From256to10(std::string str256)
{
	int64_t sum = 0;
	for (int i = 0; i < (int)str256.size(); i++)
	{
		sum = sum * 256 + (unsigned char)str256[i];
	}
	return sum;
}

void CQTool::AddLog(int32_t AuthCode, int32_t priority, const char *category, const char *content)
{
	CQ_addLog(AuthCode, priority, category, content);
}



