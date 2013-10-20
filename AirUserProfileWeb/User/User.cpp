// User.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <stdio.h>
#include <memory.h>

#ifndef UnixPlatform
#include <conio.h>
#include <windows.h>

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
				"UserPL=%d\n"
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

int _tmain(int argc, _TCHAR* argv[])
{
	bool ret;
	unsigned int handle;
	C_PortableTime tmp_time = {0}, req_time = {0};
	char remote_addr[300] = "ans.dynamas.com.tw:17862";
	char *env_user = getenv("LOGIN_USER");
	char *env_group = getenv("LOGIN_GROUP");
	char *env_qs = strdup(getenv("QUERY_STRING"));
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

	fprintf(stderr, "%s\n", env_qs);
	if(!qs_Action)
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("No action: %S\n")
			_T("failed\n"), env_qs);
		return 0;
	}
	else if(strcasecmp(qs_Action, "info") == 0)
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("LoginUser=%S\nGroup=%S\n"), env_user, env_group);
		return 0;
	}

	if(!qs_Username && strcasecmp(qs_Action, "list") != 0)
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("No action: no specified username\n")
			_T("failed\n"));
		return 0;
	}

	if(strcasecmp(env_group, "doctor") == 0 && qs_Username && strcasecmp(env_user, qs_Username) != 0)
	{
		_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
			_T("Action failed: %S could only get/set its profile (you modify %S)\n")
			_T("failed\n"), env_user, qs_Username);
		return 0;
	}

#if 1
	handle=NA_CreateObject(0);
	ret = NA_Connect(handle, remote_addr, NULL );
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
				_tprintf(_T("%S"), am_data);
			}
			else
			{
				_tprintf(_T("failed\n"));
			}
		}
		else if(strcasecmp(qs_Action, "list") == 0 && env_user)
		{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n"));
			if(strcasecmp(env_group, "admin") != 0)
			{
				_tprintf(_T("failed\n"));
			}
			else
			{
				int am_type;
				UByte am_data[MAX_USER_LIST_SIZE] = {0};
				int am_data_len = sizeof(am_data);
				int am_data_res_len;
				NA_CommunityListUser(handle, req_time, env_user, &am_type, am_data, am_data_len, &am_data_res_len);
				int len = strlen((char *)am_data);
				//mg_url_decode((char *)am_data, len + 1, (char *) am_data, len + 1, 0);
				_tprintf(_T("%S"), am_data);
			}
		}
		else if(strcasecmp(qs_Action, "add") == 0 && qs_Username && qs_PlainPassword) // add user
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
					int len = sprintf(profile, "PlainPassword=%s\n"
								"UserPL=%d\n"
								"Nickname=%s\n"
								"StatusComment=%s\n"
								"FriendList=%s\n"
								"BlackList=%s\n"
								"ServerIndex=%d\n"
								"NodeIndex=%d\n"
								"QueryString=%s\n",
								ui.PlainPassword,
								ui.UserPL,
								ui.Nickname,
								ui.StatusComment,
								ui.FriendList,
								ui.BlackList,
								ui.ServerIndex,
								ui.NodeIndex, env_user);
					if(NA_CommunityModifyUser(handle, req_time, qs_Username, (char*)profile, len+1))
					{
						//mg_url_decode(profile, len + 1, (char *) profile, len + 1, 0);
						_tprintf(_T("%S\nok\n"), profile);
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
			UByte am_data[MAX_USER_PROFILE_LEN] = {0};
			int am_data_len = sizeof(am_data);
			int am_data_res_len;
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
				_T("update user\n"));
			if(NA_CommunityQueryUser(handle, tmp_time, qs_Username, &access, &priv, &ser_idx, &node_idx, &last_login_time, &am_type, am_data, am_data_len, &am_data_res_len))
			{
				char profile[MAX_USER_PROFILE_LEN];
				struct user_info ui;
				get_user_info(&ui, (char*)am_data);
				int len = sprintf(profile, "PlainPassword=%s\n"
							"UserPL=%d\n"
							"Nickname=%s\n"
							"StatusComment=%s\n"
							"FriendList=%s\n"
							"BlackList=%s\n"
							"ServerIndex=%d\n"
							"NodeIndex=%d\n"
							"QueryString=%s\n",
							qs_PlainPassword ? qs_PlainPassword : ui.PlainPassword,
							qs_UserPL ? atoi(qs_UserPL) : ui.UserPL,
							qs_Nickname ? qs_Nickname : ui.Nickname,
							qs_StatusComment ? qs_StatusComment : ui.StatusComment,
							qs_FriendList ? qs_FriendList : ui.FriendList,
							qs_BlackList ? qs_BlackList : ui.BlackList,
							ui.ServerIndex,
							ui.NodeIndex, ui.QueryString);
				if(NA_CommunityModifyUser(handle, req_time, qs_Username, profile, len+1))
				{
					// mg_url_decode(profile, len + 1, (char *) profile, len + 1, 0);
					_tprintf(_T("%S\nok\n"), profile);
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
		else
		{
			_tprintf(_T("Content-Type: text/plain;charset='utf-8'\n\n")
				_T("Command error: %S\n")
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
	
	//printf("%s", env_qs);
	//printf("%s\n", getenv("DOCUMENT_ROOT")); //test code
#endif
	return 0;
}

