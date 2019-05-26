#pragma once

#include <string>

struct CQ_TYPE_QQ
{
	int64_t							QQID;	//QQ��
	std::string						nick;	//�ǳ�
	int								sex;	//�Ա�
	int								age;	//����
};

struct CQ_Type_GroupMember
{
	int64_t				GroupID;					// Ⱥ��
	int64_t			   	QQID;						// QQ��
	std::string			username;					// QQ�ǳ�
	std::string			nick;						// Ⱥ��Ƭ
	int					sex;						// �Ա� 0/�� 1/Ů
	int					age;						// ����
	std::string         area;						// ����
	std::string			jointime;					// ��Ⱥʱ��
	int				lastsent ;					// �ϴη���ʱ��
	std::string			level_name;					// ͷ������
	int				permission;					// Ȩ�޵ȼ� 1/��Ա 2/����Ա 3/Ⱥ��
	bool				unfriendly;					// ������Ա��¼
	std::string			title;						// �Զ���ͷ��
	int				titleExpiretime;			// ͷ�ι���ʱ��
	bool				nickcanchange;				// ����Ա�Ƿ���Э������
};
class CQTool
{
public:
	CQTool();
	~CQTool();

	bool GetGroupMemberInfo(int auth, int64_t groupID, int64_t qqid, CQ_Type_GroupMember& groupMember);
	void AddLog(int32_t AuthCode, int32_t priority, const char *category, const char *content);
private:
	bool CQ_Tools_TextToGroupMember(std::string decodeStr, CQ_Type_GroupMember& groupMember);
	int64_t CoolQ_Tools_GetNum(int index, long length, const std::string &decodeStr);
	std::string CoolQ_Tools_GetStr(int index, const std::string &decodeStr);
	int64_t From256to10(std::string str256);

	int authCode;
};

