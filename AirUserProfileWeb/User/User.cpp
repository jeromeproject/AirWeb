// User.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <stdio.h>
#include <memory.h>
#include <list>
#include <string>
#include <iostream>

#if _MSC_VER > 1020   // if VC++ version is > 4.2
   using namespace std;  // std c++ libs implemented in std
#endif

#ifndef UnixPlatform
#include <conio.h>
//#include <windows.h>
#include "CLogFile.h"
#include "minIni.h"

#ifdef DEBUG_TRACE
#undef DEBUG_TRACE
#define DEBUG_TRACE(x)
#else
#if defined(DEBUG)
#define DEBUG_TRACE(x) do { \
  flockfile(stdout); \
  printf("*** %lu.%p.%s.%d: ", \
         (unsigned long) time(NULL), (void *) pthread_self(), \
         __func__, __LINE__); \
  printf x; \
  putchar('\n'); \
  fflush(stdout); \
  funlockfile(stdout); \
} while (0)
#else
#define DEBUG_TRACE(x)
#endif // DEBUG
#endif // DEBUG_TRACE

#define strcasecmp stricmp
#define strncasecmp  strnicmp 
char* strcasestr(const char *dst, const char *src) {
        int len, dc, sc;

        if(src[0] == '\0')
                return (char*)(uintptr_t)dst;

        len = strlen(src) - 1;
        sc  = tolower(src[0]);
        for(; (dc = *dst); dst++) {
                dc = tolower(dc);
                if(sc == dc && (len == 0 || !strncasecmp(dst+1, src+1, len)))
                        return (char*)(uintptr_t)dst;
        }
        return NULL;
}
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include "NetAccess.h"
#include "NetAccess_ex.h"

static char *get_var_from_qs(char *qs, const char *name)
{
	if(!qs || !name)
		return NULL;
	char *p = strcasestr(qs, name);
	if(!p)
		return NULL;
	char *s = (p + strlen(name));
	if( *(s) == '=' )
	{
		int len;
		s += 1;
		char *e = strcasestr(s, "&");
		if(e)
			len = e - s;
		else
			len = strlen(s);

		char *buf = (char *)malloc(sizeof(char) * (len + 1 + 16 /*add extra small buffer for other changes*/));
		strncpy(buf, s, len);
		buf[len] = '\0';
		return buf;
	}
	return NULL;
}

static char * strip_start_symbol(char *str, char c)
{
	if(*str == c)
	{
		strcpy(str, str+1);
	}
	return str;
}

static bool get_user_info(struct user_info *ui, char *am_data)
{
	bool ret = sscanf(am_data, "PlainPassword%[^\n]\n"
				"UserPL=%d\n"	// 0 1 2 3
				"Nickname%[^\n]\n"
				"StatusComment%[^\n]\n"
				"FriendList%[^\n]\n"
				"BlackList%[^\n]\n"
				"ServerIndex=%d\n"
				"NodeIndex=%d\n"
				"QueryString%[^\n]\n",
				ui->PlainPassword,
				&ui->UserPL,
				ui->Nickname,
				ui->StatusComment,
				ui->FriendList,
				ui->BlackList,
				&ui->ServerIndex,
				&ui->NodeIndex,
				ui->QueryString) == 9 ? true : false;
	if(ret)
	{
		strip_start_symbol(ui->PlainPassword, '=');
		strip_start_symbol(ui->Nickname, '=');
		strip_start_symbol(ui->StatusComment, '=');
		strip_start_symbol(ui->FriendList, '=');
		strip_start_symbol(ui->BlackList, '=');
		strip_start_symbol(ui->QueryString, '=');
	}
	return ret;
}

int mg_url_decode(const char *src, int src_len, char *dst,
                  int dst_len, int is_form_url_encoded) {
  int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

  for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
    if (src[i] == '%' && i < src_len - 2 &&
        isxdigit(* (const unsigned char *) (src + i + 1)) &&
        isxdigit(* (const unsigned char *) (src + i + 2))) {
      a = tolower(* (const unsigned char *) (src + i + 1));
      b = tolower(* (const unsigned char *) (src + i + 2));
      dst[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
      i += 2;
    } else if (is_form_url_encoded && src[i] == '+') {
      dst[j] = ' ';
    } else {
      dst[j] = src[i];
    }
  }

  dst[j] = '\0'; // Null-terminate the destination

  return i >= src_len ? j : -1;
}

/*
bool NA_GetTextSetting( unsigned int handle, int type1, int type2, UDWord data1, UDWord data2, char *setting_buf, int setting_buf_size );
handle: 介面程式物件的handle值
type1: 指定的設定值型態
type2、data1、data2: 其他可有可無的的選項參數
setting_buf: 傳回文字設定值資料的緩衝區位址
setting_buf_size: 所提供緩衝區的大小

> netcon -gs dynamas.com.tw:17861 3 17 0 0
NS_17_IsEnable=1
NS_17_TargetType=0
NS_17_SubTargetType=10000
NS_17_IPAddress=_D_1C7B21594E84
NS_17_PortNumber=17860
NS_17_Lookup=0
NS_17_WaitForConnect=1
NS_17_UserName=Admin
NS_17_Password=
NS_17_TrackMessage=1
NS_17_TrackEvent=1
NS_17_TrackGPS=1
NS_17_AutoConnect=1
NS_17_AutoAdjustTime=0
NS_17_AutoImageRecovery=0
*/
static bool get_rms_node_channel_info(struct rms_node_info *ni, char *setting_buf)
{
	int ch;
	bool ret = sscanf(setting_buf, 
				"NS_%d_IsEnable=%d\n"
				"NS_%d_TargetType=%d\n"
				"NS_%d_SubTargetType=%d\n"
				"NS_%d_IPAddress%[^\n]\n"
				"NS_%d_PortNumber=%d\n"
				"NS_%d_Lookup=%d\n"
				"NS_%d_WaitForConnect=%d\n"
				"NS_%d_UserName%[^\n]\n"
				"NS_%d_Password%[^\n]\n"
				"NS_%d_TrackMessage=%d\n"
				"NS_%d_TrackEvent=%d\n"
				"NS_%d_TrackGPS=%d\n"
				"NS_%d_AutoConnect=%d\n"
				"NS_%d_AutoAdjustTime=%d\n"
				"NS_%d_AutoImageRecovery=%d\n",
				&ch, &ni->IsEnable,
				&ch, &ni->TargetType,
				&ch, &ni->SubTargetType,
				&ch, ni->IPAddress,
				&ch, &ni->PortNumber,
				&ch, &ni->Lookup,
				&ch, &ni->WaitForConnect,
				&ch, ni->UserName,
				&ch, ni->Password,
				&ch, &ni->TrackMessage,
				&ch, &ni->TrackEvent,
				&ch, &ni->TrackGPS,
				&ch, &ni->AutoConnect,
				&ch, &ni->AutoAdjustTime,
				&ch, &ni->AutoImageRecovery) == 30 ? true : false;
	strip_start_symbol(ni->IPAddress, '=');
	strip_start_symbol(ni->UserName, '=');
	strip_start_symbol(ni->Password, '=');

	return ret;
}


int rms_node_list_cache(unsigned int handle, list<struct rms_node_info> *nl)
{
	int i, num_nodes;
	char setting_buf[2048];
	struct rms_node_info ni;
	num_nodes = NA_SysGetNodeCount(handle);

	for(i = 0; i < num_nodes; ++i)
	{
		ni.index = i;
		if(NA_GetNodeTextSetting(handle, i, setting_buf, sizeof(setting_buf)) && get_rms_node_channel_info(&ni, setting_buf))
		{
			nl->push_back(ni);
		}
	}

	return 0;
}

bool rms_node_update(unsigned int handle, struct rms_node_info *ni)
{
	char setting_buf[1024];
	int ch = ni->index;
	sprintf_s(setting_buf, sizeof(setting_buf),
				"NS_%d_IsEnable=%d\n"
				"NS_%d_TargetType=%d\n"
				"NS_%d_SubTargetType=%d\n"
				"NS_%d_IPAddress=%s\n"
				"NS_%d_PortNumber=%d\n"
				"NS_%d_Lookup=%d\n"
				"NS_%d_WaitForConnect=%d\n"
				"NS_%d_UserName=%s\n"
				"NS_%d_Password=%s\n"
				"NS_%d_TrackMessage=%d\n"
				"NS_%d_TrackEvent=%d\n"
				"NS_%d_TrackGPS=%d\n"
				"NS_%d_AutoConnect=%d\n"
				"NS_%d_AutoAdjustTime=%d\n"
				"NS_%d_AutoImageRecovery=%d\n",
				ch, ni->IsEnable,
				ch, ni->TargetType,
				ch, ni->SubTargetType,
				ch, ni->IPAddress,
				ch, ni->PortNumber,
				ch, ni->Lookup,
				ch, ni->WaitForConnect,
				ch, ni->UserName,
				ch, ni->Password,
				ch, ni->TrackMessage,
				ch, ni->TrackEvent,
				ch, ni->TrackGPS,
				ch, ni->AutoConnect,
				ch, ni->AutoAdjustTime,
				ch, ni->AutoImageRecovery);
	NA_SetNodeTextSetting(handle, ni->index, setting_buf);

	ch = ni->index*16 + 1;
	sprintf_s(setting_buf, sizeof(setting_buf),
				"CS_%d_IsEnable=1\n"
				"CS_%d_RequestAudioStream=1\n",
				ch, ch);
	CLogger::Log(0, _T("%s"), setting_buf);
	return NA_SetChannelTextSetting(handle, ch, setting_buf);
}

char* str_append(char* orig, const char* str, const char *split)
{
	if(orig[0] == 0)
	{
		return strcpy(orig, str);
	}
	else
	{
		strcat(orig, split);
		return strcat(orig, str);
	}
}

list<char *> str_spliter(char *str, char *spliter)
{
	list<char *> obj;
	char *token, *ptr;
	for(token = strtok_s(str, spliter, &ptr); token; token = strtok_s(NULL, spliter, &ptr))
	{
		obj.push_back(token);
	}
	return obj;
}

bool rms_delete_node_by_ipaddress(unsigned int handle, list<struct rms_node_info> *cache_nl, char *ipaddr)
{
	list<struct rms_node_info>::iterator iter;

	for(iter = cache_nl->begin(); iter != cache_nl->end(); ++iter)
	{
		if(strcmp(iter->IPAddress, ipaddr) == 0)
		{
			iter->IsEnable = 0;
			strcpy(iter->IPAddress, "");
			return (rms_node_update(handle, &(*iter)));
		}
	}
}

char *str_add_quot(char *str)
{
	int len = strlen(str);
	int i;
	if(len == 0)
	{
		strcpy(str, "\"\"");
		return str;
	}
	if(str[0] != '"')
	{
		for(i = len; i > 0; --i)
			str[i] = str[i-1];
		str[0] = '"';
	}
	else
		len -= 1;	//the last position was not change
	if(str[len] != '"')
	{
		str[len+1] = '"';
		str[len+2] = 0;
	}
	else
		str[len+1] = 0;
	return str;
}

char *str_rm_quot(char *str)
{
	int len = strlen(str);
	int i, j;
	for(i = 0,  j = 0; i < len; ++i)
		if(str[i] != '"')
			str[j++] = str[i];
	str[j] = 0;
	return str;
}
#define FRIEND_LIST_SPLIT_CHAR " "
#define FRIEND_LIST_SPLIT_CHARS ",; "
void rms_removed_items_by_diff(unsigned int handle, list<struct rms_node_info> *nl, list<char*> *new_add_list, list<char*> *old_exist_list)
{
	bool found;
	list<char*>::iterator niter, miter;
	for(niter = old_exist_list->begin(); niter != old_exist_list->end(); ++niter)
	{
		found = false;
		// delete for each fd in old friend list but not in new firend list
		for(miter = new_add_list->begin(); miter != new_add_list->end(); ++miter)
		{
			if(strcasecmp(*niter, *miter) == 0)
			{
				found = true;
				break;
			}
		}
		if(!found)	// the node was not found in new list
		{
			// to delete the node from rms
			if(rms_delete_node_by_ipaddress(handle, nl, *niter))
			{
				// log deleted node
				CLogger::Log(0, _T("removed node for %s"), *niter);
			}
			else
			{
				// log can't delete node
				CLogger::Log(0, _T("failed to remove node for %s"), *niter);
			}
		}
	}
}
/*
	rms_update_node_list
	0 ok
	-1 (NUode Full)
*/
int rms_update_node_list(unsigned int handle, char *new_list /*final result*/, char *old_list)
{
	bool found;
	char *node, *ptr, bak_list[2048];
	list<struct rms_node_info> nl;
	list<struct rms_node_info>::iterator iter, empty_iter;
	list<char*>::iterator niter;
	// cache list from rms
	rms_node_list_cache(handle, &nl);
	str_rm_quot(new_list);
	str_rm_quot(old_list);
	strcpy(bak_list, new_list);
	CLogger::Log(0, _T("new list=%s, old_list=%s"), new_list, old_list);
	list<char*> valid_list_array;
	list<char*> new_list_array = str_spliter(bak_list, FRIEND_LIST_SPLIT_CHARS);
	list<char*> old_list_array = str_spliter(old_list, FRIEND_LIST_SPLIT_CHARS);
	// first removed the remover to make space for new
	rms_removed_items_by_diff(handle, &nl, &new_list_array, &old_list_array);
	//for(node = strtok_s(bak_list, FRIEND_LIST_SPLIT_CHARS, &ptr); node; node = strtok_s(NULL, FRIEND_LIST_SPLIT_CHARS, &ptr))
	for(niter = new_list_array.begin(); niter != new_list_array.end(); ++niter)
	{
		found = false;
		// for each fd in friend list
		for(iter = nl.begin(), empty_iter = nl.end(); iter != nl.end(); ++iter)
		{
			if(empty_iter == nl.end() && strcmp(iter->IPAddress, "") == 0)
				empty_iter = iter;
			else if(strcmp(iter->IPAddress, *niter) == 0)
			{
				found = true;
				// break if IPAddress is found in node list
				break;
			}
		}
		if(!found)
		{
			if(empty_iter != nl.end())
			{
				// set ns_#_ipaddress to new fd if this node is not exist
				empty_iter->IsEnable = 1;
				strcpy(empty_iter->IPAddress, *niter);
				strcpy(empty_iter->UserName, "Admin");
				strcpy(empty_iter->Password, "");
				empty_iter->Lookup = 0;
				empty_iter->WaitForConnect = 1;
				empty_iter->TrackGPS = 1;
				if(rms_node_update(handle, &(*empty_iter)))
				{
					// log new node if added suuccessfully
					valid_list_array.push_back(*niter);
					CLogger::Log(0, _T("add new node for %s"), *niter);
				}
				else
				{
					CLogger::Log(0, _T("failed to add new node for %s"), *niter);
				}
			}
			else
			{
				// no empty node for new node
				// log there is no empty node for new node if the list is full
				CLogger::Log(0, _T("no empty node for %s"), *niter);
			}
		}
		else
		{
			// log node is exist and ignore it
			valid_list_array.push_back(*niter);
			CLogger::Log(0, _T("node of %s was exist"), *niter);
		}
	}

	// removed node
	rms_removed_items_by_diff(handle, &nl, &valid_list_array, &new_list_array);
	for(new_list[0] = 0, niter = valid_list_array.begin(); niter != valid_list_array.end(); ++niter)
	{
		//CLogger::Log(0, _T("valid item=%s"), *niter);
		str_append(new_list, *niter, FRIEND_LIST_SPLIT_CHAR);
	}
	//CLogger::Log(0, _T("valid list=%s"), new_list);
	str_add_quot(new_list);

	CLogger::Log(0, _T("valid list=%s"), new_list);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool ret;
	minIni ini("config.ini");
	string s1, s2;
	/* string reading */
	s1 = ini.gets( "User.exe", "NAME_SERVER" , "ans.dynamas.com.tw:17862" );
	s2 = ini.gets( "User.exe", "RMS01" , "ans.dynamas.com.tw:17862" );
	unsigned int handle;	// for Name server
	unsigned int handle2;	// for RMS
	C_PortableTime tmp_time = {0}, req_time = {0};
	const char *remote_addr = s1.c_str();
	const char *remote_addr2 = s2.c_str();
	char *env_user = getenv("LOGIN_USER");
	char *env_group = getenv("LOGIN_GROUP");
	char *env_qs = strdup(getenv("QUERY_STRING"));
	if(!env_qs)
	{
		// test mode in console
		CLogger::Log(0, _T("get s1=%s, s2=%s"), s1.c_str(), s2.c_str());
		CLogger::Log(0, _T("test mode"));
		//env_user = strdup(_T("admin"));
		//env_group = strdup(_T("admin"));
		env_qs = strdup("Action=info&qs_Username=admin");
	}
	int len = strlen(env_qs);
	mg_url_decode(env_qs, len + 1, (char *) env_qs, len + 1, 0);
	char *qs_Action = get_var_from_qs(env_qs, "Action");
	char *qs_Username = get_var_from_qs(env_qs, "Username");
	char *qs_PlainPassword = get_var_from_qs(env_qs, "PlainPassword");
	char *qs_UserPL = get_var_from_qs(env_qs, "UserPL");
	char *qs_Nickname = get_var_from_qs(env_qs, "Nickname");
	char *qs_StatusComment = get_var_from_qs(env_qs, "StatusComment");
	char *qs_FriendList = get_var_from_qs(env_qs, "FriendList");
	char *qs_BlackList = get_var_from_qs(env_qs, "BlackList");
	char *qs_QueryString = get_var_from_qs(env_qs, "QueryString");
	fprintf(stderr, "%s\n", env_qs);
	if(!qs_Action)
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("No action: %s\n")
			_T("failed\n"), env_qs);
		return 0;
	}
	else if(strcasecmp(qs_Action, "info") == 0)
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("LoginUser=%s\nGroup=%s\n"), env_user, env_group);
		return 0;
	}

	if(!qs_Username && (strcasecmp(qs_Action, "list") != 0 && strcasecmp(qs_Action, "fdlist") != 0))
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("No action: no specified username\n")
			_T("failed\n"));
		return 0;
	}

	if(strcasecmp(env_group, "doctor") == 0 && qs_Username && strcasecmp(env_user, qs_Username) != 0)
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("Action failed: %s could only get/set its profile (you modify %s)\n")
			_T("failed\n"), env_user, qs_Username);
		return 0;
	}
	if( !(strcasecmp(qs_Action, "fdadd") == 0 || strcasecmp(qs_Action, "fdlist") == 0) && qs_QueryString && strcasecmp(env_group, "admin") != 0 && strcmp(qs_QueryString, env_user) != 0)
	{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("Permission Denied\n")
			_T("failed\n"));
		return 0;
	}
#if 1
	handle2 = handle = NA_CreateObject(0);
	ret = NA_Connect(handle, remote_addr, NULL );
	handle2 = NA_CreateObject(0);
	ret = NA_Connect(handle2, remote_addr2, NULL );

	if(ret == true)
	{
		//ret = NA_CommunityCheckLogin(handle, tmp_time, "person", "test");
		if(strcasecmp(qs_Action, "get") == 0 && qs_Username)
		{
			C_PortableTime last_login_time;
			UDWord access, priv, ser_idx, node_idx;
			int am_type;
			UByte am_data[MAX_USER_PROFILE_LEN] = {0};
			int am_data_len = sizeof(am_data);
			int am_data_res_len;
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n"));
			if(NA_CommunityQueryUser(handle, tmp_time, qs_Username, &access, &priv, &ser_idx, &node_idx, &last_login_time, &am_type, am_data, am_data_len, &am_data_res_len))
			{
				int len = strlen((char *)am_data);
				//mg_url_decode((char *)am_data, len + 1, (char *) am_data, len + 1, 0);
				_tprintf(_T("%s"), am_data);
			}
			else
			{
				_tprintf(_T("failed\n"));
			}
		}
		else if(strcasecmp(qs_Action, "list") == 0 && env_user)
		{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n"));
			if(strcasecmp(env_group, "admin") != 0 && strcasecmp(env_group, "unit") != 0)
			{
				_tprintf(_T("failed\n"));
			}
			else
			{
				int am_type;
				UByte am_data[MAX_USER_LIST_SIZE] = {0};
				int am_data_len = sizeof(am_data);
				int am_data_res_len;
				char *target = env_user;
				if(qs_QueryString)
					target = qs_QueryString;

				NA_CommunityListUser(handle, req_time, target, &am_type, am_data, am_data_len, &am_data_res_len);
				int len = strlen((char *)am_data);
				//mg_url_decode((char *)am_data, len + 1, (char *) am_data, len + 1, 0);
				_tprintf(_T("%s"), am_data);
			}
		}
		else if(strcasecmp(qs_Action, "add") == 0 && qs_Username && qs_PlainPassword && qs_UserPL) // add user
		{
			C_PortableTime last_login_time;
			UDWord access, priv, ser_idx, node_idx;
			int am_type;
			UByte am_data[MAX_USER_PROFILE_LEN] = {0};
			int am_data_len = sizeof(am_data);
			int am_data_res_len;
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
				_T("create user\n"));
			if(NA_CommunityCreateUser(handle, tmp_time, qs_Username, qs_PlainPassword))
			{
				if(NA_CommunityQueryUser(handle, tmp_time, qs_Username, &access, &priv, &ser_idx, &node_idx, &last_login_time, &am_type, am_data, am_data_len, &am_data_res_len))
				{
					char profile[MAX_USER_PROFILE_LEN];
					struct user_info ui;
					get_user_info(&ui, (char*)am_data);
					char *target = env_user;
					if(qs_QueryString)
						target = qs_QueryString;
					int len = sprintf(profile, "PlainPassword=%s\n"
								"UserPL=%s\n"
								"Nickname=%s\n"
								"StatusComment=%s\n"
								"FriendList=%s\n"
								"BlackList=%s\n"
								"ServerIndex=%d\n"
								"NodeIndex=%d\n"
								"QueryString=%s\n",
								ui.PlainPassword,
								qs_UserPL,
								ui.Nickname,
								ui.StatusComment,
								ui.FriendList,
								ui.BlackList,
								ui.ServerIndex,
								ui.NodeIndex, target);
					if(NA_CommunityModifyUser(handle, req_time, qs_Username, (char*)profile, len+1))
					{
						//mg_url_decode(profile, len + 1, (char *) profile, len + 1, 0);
						_tprintf(_T("%s\nok\n"), profile);
					}
					else
					{
						_tprintf(_T("failed\n"));
					}
				}
				else
					_tprintf(_T("failed\n"));
			}
			else
				_tprintf(_T("failed\n"));
		}
		else if(strcasecmp(qs_Action, "update") == 0 && qs_Username) // update user
		{
			C_PortableTime last_login_time;
			UDWord access, priv, ser_idx, node_idx;
			int am_type;
			char am_data[MAX_USER_PROFILE_LEN] = {0};
			int am_data_len = sizeof(am_data);
			int am_data_res_len;
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
				_T("update user\n"));
			if(NA_CommunityQueryUser(handle, tmp_time, qs_Username, &access, &priv, &ser_idx, &node_idx, &last_login_time, &am_type, (UByte*)am_data, am_data_len, &am_data_res_len))
			{
				bool missing_data = false;
				char profile[MAX_USER_PROFILE_LEN];
				struct user_info ui;
				am_data[am_data_res_len] = 0;
				CLogger::Log(0, _T("get setting len: %d"), am_data_res_len);
				CLogger::Log(0, _T("get setting val: %s"), am_data);
				get_user_info(&ui, (char*)am_data);
				if(qs_FriendList)
				{
					char friend_list[sizeof(ui.FriendList)];
					strcpy(friend_list, qs_FriendList);
					rms_update_node_list(handle2, friend_list, ui.FriendList);
					if(strcasecmp(friend_list, qs_FriendList) != 0)
					{
						missing_data = true;
						strcpy(qs_FriendList, friend_list);
					}
				}
				int len = sprintf(profile, "PlainPassword=%s\n"
							"UserPL=%d\n"
							"Nickname=%s\n"
							"StatusComment=%s\n"
							"FriendList=%s\n"
							"BlackList=%s\n"
							"ServerIndex=%d\n"
							"NodeIndex=%d\n"
							,//"QueryString=%s\n",
							qs_PlainPassword ? qs_PlainPassword : ui.PlainPassword,
							qs_UserPL ? atoi(qs_UserPL) : ui.UserPL,
							qs_Nickname ? qs_Nickname : ui.Nickname,
							qs_StatusComment ? qs_StatusComment : ui.StatusComment,
							qs_FriendList ? qs_FriendList : ui.FriendList,
							qs_BlackList ? qs_BlackList : ui.BlackList,
							ui.ServerIndex,
							ui.NodeIndex);//, ui.QueryString);
				CLogger::Log(0, _T("set setting: %s"), profile);
				if(NA_CommunityModifyUser(handle, req_time, qs_Username, profile, len+1))
				{
					// mg_url_decode(profile, len + 1, (char *) profile, len + 1, 0);
					_tprintf(_T("%s\nok\n"), profile);
					if(missing_data)
					{
						CLogger::Log(0, _T("missing friend list"));
						_tprintf(_T("showtxt:(Node Full)\n"));
					}
				}
				else
				{
					_tprintf(_T("failed\n"));
				}
			}
			else
			{
				_tprintf(_T("failed\n"));
			}
		}
		else if(strcasecmp(qs_Action, "del") == 0 && qs_Username) // del user
		{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
				_T("delete user\n"));
			if(NA_CommunityDeleteUser(handle, tmp_time, qs_Username))
			{
				_tprintf(_T("ok\n"));
			}
			else
			{
				_tprintf(_T("failed\n"));
			}
		}
		// friend command
		//http://127.0.0.1:8080/User.exe?action=fdlist&QueryString=PersonTest
		else if(strcasecmp(qs_Action, "fdlist") == 0 && env_user)
		{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n"));
			// check privilege
			/*
			if(strcasecmp(env_group, "admin") != 0 || (strcasecmp(env_group, "unit") == 0 && strcasecmp(env_user ,qs_QueryString) ||
				(strcasecmp(env_group, "doctor") == 0 && strcasecmp("doctor's unit" ,qs_QueryString)))
			{
				_tprintf(_T("failed\n"));
			}
			else
			*/
			{
				int am_type;
				UByte* am_data = (UByte*)malloc(MAX_USER_LIST_SIZE);
				int am_data_len = MAX_USER_LIST_SIZE;
				int am_data_res_len;
				char *target = env_user;
				if(qs_QueryString)
					target = qs_QueryString;
				target = "global";

				NA_CommunityListDevice(handle, req_time, target, &am_type, am_data, am_data_len, &am_data_res_len);
				int len = strlen((char *)am_data);
				char *x = strdup((char *)am_data);
				char *ptr, *device_str;
				//mg_url_decode((char *)am_data, len + 1, (char *) am_data, len + 1, 0);
				for(device_str = strtok_s(x, "\n", &ptr); device_str; device_str = strtok_s(NULL, "\n", &ptr))
				{
					if(NA_CommunityQueryDevice(handle, tmp_time, device_str, &am_type, am_data, am_data_len, &am_data_res_len))
					{
						char profile[1280];
						struct user_info ui;
						get_user_info(&ui, (char*)am_data);
						int len = sprintf(profile, "%s %s\n",
							device_str, ui.Nickname);
						//CLogger::Log(0, _T("get friend setting val: %s"), profile);
						_tprintf(_T("%s"), profile);
					}
				}
				free(x);
				free(am_data);
				//_tprintf(_T("%s"), am_data);
			}
		}
		//http://127.0.0.1:8080/User.exe?action=fdadd&QueryString=PersonTest&Nickname=%22hid1234%20person%22&Username=_D_person1
		else if(strcasecmp(qs_Action, "fdadd") == 0 && qs_Username) // add friend
		{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n"));
			/*
			if(strcasecmp(env_group, "admin") != 0 || (strcasecmp(env_group, "unit") == 0 && strcasecmp(env_user ,qs_QueryString) ||
				(strcasecmp(env_group, "doctor") == 0 && strcasecmp("doctor's unit" ,qs_QueryString)))
			*/
			CLogger::Log(0, _T("create new deivce=%s, nickname=%s, belongs to=%s\n"), qs_Username, qs_Nickname, qs_QueryString);
			if(qs_QueryString && qs_Nickname && qs_Username)
			{
				int am_type;
				char init_setting[256];
				UByte am_data[MAX_USER_PROFILE_LEN] = {0};
				int am_data_len = sizeof(am_data);
				int am_data_res_len;
				_tprintf(_T("create device\n"));
				sprintf(init_setting, "NameServerAddress=%s", s2.c_str());
				if(NA_CommunityCreateDevice(handle, tmp_time, qs_Username, init_setting))
				{
					if(NA_CommunityQueryDevice(handle, tmp_time, qs_Username, &am_type, am_data, am_data_len, &am_data_res_len))
					{
						char profile[MAX_USER_PROFILE_LEN];
						struct user_info ui;
						//get_friend_info(&ui, (char*)am_data);

						char *target = qs_QueryString;
						target = "global";
						char *nickname = qs_Nickname;
						int len = sprintf(profile, "Nickname=%s\n"
									"QueryString=%s\n",
									nickname,
									target);
						if(NA_CommunityModifyDevice(handle, req_time, qs_Username, (char*)profile, len+1))
						{
							CLogger::Log(0, _T("add friend setting val: %s"), profile);
							//mg_url_decode(profile, len + 1, (char *) profile, len + 1, 0);
							_tprintf(_T("%s\nok\n"), profile);
						}
						else
						{
							_tprintf(_T("NA_CommunityModifyDevice failed\n"));
						}
					}
					else
						_tprintf(_T("NA_CommunityQueryDevice failed\n"));
				}
				else
					_tprintf(_T("NA_CommunityCreateDevice failed\n"));
			}
			else
				_tprintf(_T("failed\n"));
		}
		//http://127.0.0.1:8080/User.exe?action=fddel&Username=_D_person1
		else if(strcasecmp(qs_Action, "fddel") == 0 && qs_Username) // del device
		{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
				_T("delete device\n"));
			if(NA_CommunityDeleteDevice(handle, tmp_time, qs_Username))
			{
				_tprintf(_T("ok\n"));
			}
			else
			{
				_tprintf(_T("failed\n"));
			}
		}
		else
		{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
				_T("Command error: %s\n")
				_T("failed\n"), env_qs);
		}
		NA_Disconnect(handle);
	}
	else
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("Disconnected\n")
			_T("failed\n"));
	}
	NA_DestroyObject(handle);
	NA_DestroyObject(handle2);

	//printf("%s", env_qs);
	//printf("%s\n", getenv("DOCUMENT_ROOT")); //test code
#endif
	return 0;
}

