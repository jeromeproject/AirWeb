// User.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <stdio.h>
#include <memory.h>

#ifndef UnixPlatform
#include <conio.h>
#include <windows.h>

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

#include "NetAccess_ex.h"
#include "NetAccess.h"

// UDWord NA_ExtendedCommand( unsigned int handle, int p1, UDWord p2, UDWord p3, UDWord p4, C_PortableTime time, const char *text_info_1, const char *text_info_2, UDWord *data1_ptr, UDWord *data2_ptr, UDWord *data3_ptr, UDWord *data4_ptr, C_PortableTime *time_ptr, char *path_filename_buf_ptr, int pfn_buf_size, C_PortableTime *am_time_ptr, int *am_type1_ptr, int *am_type2_ptr, UDWord *am_ext_type1_ptr, UDWord *am_ext_type2_ptr, UByte *data_buf1_ptr, UByte *data_buf2_ptr, int data_buf1_size, int data_buf2_size, int *data_size1_ptr, int *data_size2_ptr )
#define NA_CommunityCheckLogin(handle, req_time, username, password) NA_ExtendedCommand(handle, Ex_CommunityCheckLogin, 0, 0, 0, req_time, username, password, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)
#define NA_CommunityQueryUser(handle, req_time, username, access, priv, ser_idx, node_idx, last_login_time, am_type, am_data, am_data_len, am_data_res_len) NA_ExtendedCommand(handle, Ex_CommunityQueryUser, 0, 0, 0, req_time, username, NULL, access, priv, ser_idx, node_idx, last_login_time, NULL, 0,0, am_type, 0,0,0,am_data,0,am_data_len,0,am_data_res_len,0)
#define NA_CommunityCreateUser(handle, req_time, username, password) NA_ExtendedCommand(handle, Ex_CommunityCreateUser, 0, 0, 0, req_time, username, password, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)
#define NA_CommunityDeleteUser(handle, req_time, username) NA_ExtendedCommand(handle, Ex_CommunityDeleteUser, 0, 0, 0, req_time, username, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)
#define NA_CommunityModifyUser(handle, req_time, username, profile, len_profile) NA_ExtendedCommand(handle, Ex_CommunityModifyUser, 0, 0, 0, req_time, username, profile, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)

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
		s += 1;
		char *e = strcasestr(s, "&");
		if(e)
		{
			int len = e - s;

			char *buf = (char *)malloc(sizeof(char) * (len + 1));
			strncpy(buf, s, len);
			buf[len] = '\0';
			return buf;
		}
		else
		{
			return strdup(s);
		}
	}
	return NULL;
}

char * strip_start_symbol(char *str, char c)
{
	if(*str == c)
	{
		strcpy(str, str+1);
	}
	return str;
}
/*
PlainPassword=test
UserPL=-1
Nickname=
StatusComment=
FriendList=
BlackList=
ServerIndex=-1
NodeIndex=-1
*/
struct user_info {
	char PlainPassword[128];
	int UserPL;
	char Nickname[128];
	char StatusComment[128];
	char FriendList[14000];
	char BlackList[14000];
	int ServerIndex;
	int NodeIndex;
};

#define MAX_PROFILE_LEN sizeof(struct user_info)

bool get_user_info(struct user_info *ui, char *am_data)
{
	bool ret = sscanf(am_data, "PlainPassword%[^\n]\n"
				"UserPL=%d\n"
				"Nickname%[^\n]\n"
				"StatusComment%[^\n]\n"
				"FriendList%[^\n]\n"
				"BlackList%[^\n]\n"
				"ServerIndex=%d\n"
				"NodeIndex=%d\n",
				ui->PlainPassword,
				&ui->UserPL,
				ui->Nickname,
				ui->StatusComment,
				ui->FriendList,
				ui->BlackList,
				&ui->ServerIndex,
				&ui->NodeIndex) == 8 ? true : false;
	if(ret)
	{
		strip_start_symbol(ui->PlainPassword, '=');
		strip_start_symbol(ui->Nickname, '=');
		strip_start_symbol(ui->StatusComment, '=');
		strip_start_symbol(ui->FriendList, '=');
		strip_start_symbol(ui->BlackList, '=');
	}
	return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool ret;
	unsigned int handle;
	C_PortableTime tmp_time = {0}, req_time = {0};
	char remote_addr[300] = "ans.dynamas.com.tw:17862";
	handle=NA_CreateObject(0);
	char *env_user = getenv("LOGIN_USER");
	char *env_passwd = getenv("LOGIN_PASSWORD");
	char *env_group = getenv("LOGIN_GROUP");
	char *env_qs = getenv("QUERY_STRING");
	char *qs_Action = get_var_from_qs(env_qs, "Action");
	char *qs_Username = get_var_from_qs(env_qs, "Username");
	char *qs_PlainPassword = get_var_from_qs(env_qs, "PlainPassword");
	char *qs_UserPL = get_var_from_qs(env_qs, "UserPL");
	char *qs_Nickname = get_var_from_qs(env_qs, "Nickname");
	char *qs_StatusComment = get_var_from_qs(env_qs, "StatusComment");
	char *qs_FriendList = get_var_from_qs(env_qs, "FriendList");
	char *qs_BlackList = get_var_from_qs(env_qs, "BlackList");

	if(!qs_Action)
	{
		printf("Content-Type: text/plain\n"
			"\n"
			"No action: %s\n"
			"failed\n", env_qs);
		return 0;
	}

#if 1
	ret = NA_Connect(handle, remote_addr, NULL );
	if(ret == true)
	{
		//ret = NA_CommunityCheckLogin(handle, tmp_time, "person", "test");
		if(strcasecmp(qs_Action, "get") == 0 && qs_Username)
		{
			C_PortableTime last_login_time;
			UDWord access, priv, ser_idx, node_idx;
			int am_type;
			UByte am_data[MAX_PROFILE_LEN] = {0};
			int am_data_len = sizeof(am_data);
			int am_data_res_len;
			printf("Content-Type: text/plain\n\n");
			if(NA_CommunityQueryUser(handle, tmp_time, qs_Username, &access, &priv, &ser_idx, &node_idx, &last_login_time, &am_type, am_data, am_data_len, &am_data_res_len))
			{
				printf("%s", am_data);
			}
			else
			{
				printf("failed\n");
			}
		}
		else if(strcasecmp(qs_Action, "add") == 0 && qs_Username && qs_PlainPassword) // add user
		{
			C_PortableTime last_login_time;
			UDWord access, priv, ser_idx, node_idx;
			int am_type;
			UByte am_data[MAX_PROFILE_LEN] = {0};
			int am_data_len = sizeof(am_data);
			int am_data_res_len;
			printf("Content-Type: text/plain\n"
				"\n"
				"create user\n");
			if(NA_CommunityCreateUser(handle, tmp_time, qs_Username, qs_PlainPassword))
			{
				if(NA_CommunityQueryUser(handle, tmp_time, qs_Username, &access, &priv, &ser_idx, &node_idx, &last_login_time, &am_type, am_data, am_data_len, &am_data_res_len))
				{
					struct user_info ui;
					get_user_info(&ui, (char*)am_data);
					printf("ok\n");
				}
				else
					printf("failed\n");
			}
			else
				printf("failed\n");
		}
		else if(strcasecmp(qs_Action, "update") == 0 && qs_Username) // update user
		{
			C_PortableTime last_login_time;
			UDWord access, priv, ser_idx, node_idx;
			int am_type;
			UByte am_data[MAX_PROFILE_LEN] = {0};
			int am_data_len = sizeof(am_data);
			int am_data_res_len;
			printf("Content-Type: text/plain\n"
				"\n"
				"update user\n");
			if(NA_CommunityQueryUser(handle, tmp_time, qs_Username, &access, &priv, &ser_idx, &node_idx, &last_login_time, &am_type, am_data, am_data_len, &am_data_res_len))
			{
				char profile[MAX_PROFILE_LEN];
				struct user_info ui;
				get_user_info(&ui, (char*)am_data);
				int len = sprintf(profile, "PlainPassword=%s\n"
							"UserPL=%d\n"
							"Nickname=%s\n"
							"StatusComment=%s\n"
							"FriendList=%s\n"
							"BlackList=%s\n"
							"ServerIndex=%d\n"
							"NodeIndex=%d\n",
							qs_PlainPassword ? qs_PlainPassword : ui.PlainPassword,
							qs_UserPL ? atoi(qs_UserPL) : ui.UserPL,
							qs_Nickname ? qs_Nickname : ui.Nickname,
							qs_StatusComment ? qs_StatusComment : ui.StatusComment,
							qs_FriendList ? qs_FriendList : ui.FriendList,
							qs_BlackList ? qs_BlackList : ui.BlackList,
							ui.ServerIndex,
							ui.NodeIndex);
				if(NA_CommunityModifyUser(handle, req_time, qs_Username, profile, len+1))
				{
					printf("ok\n");
				}
				else
				{
					printf("failed\n");
				}
			}
			else
			{
				printf("failed\n");
			}
		}
		else if(strcasecmp(qs_Action, "del") == 0 && qs_Username) // del user
		{
			printf("Content-Type: text/plain\n"
				"\n"
				"delete user\n");
			if(NA_CommunityDeleteUser(handle, tmp_time, qs_Username))
			{
				printf("ok\n");
			}
			else
			{
				printf("failed\n");
			}
		}
		else
		{
			printf("Content-Type: text/plain\n"
				"\n"
				"Command error: %s\n"
				"failed\n", env_qs);
		}
		NA_Disconnect(handle);
	}
	else
	{
		printf("Content-Type: text/plain\n"
			"\n"
			"Disconnected\n"
			"failed\n");
	}
	NA_DestroyObject(handle);
	
	//printf("%s", env_qs);
	//printf("%s\n", getenv("DOCUMENT_ROOT")); //test code
#endif
	return 0;
}

