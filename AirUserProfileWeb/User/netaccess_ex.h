enum SysCommandType {
  Sys_Reboot=0,
      // ���ݨt�έ��}��
      // Reboot the remote station
  Sys_ShutDown,
      // ���滷�ݹq������
      // Shut down the remote station
  Sys_TrackMessage,
      // p2=track mask, p3=option flags(reserved), p4=command data, Return=true/false
  Sys_GetAdvertiseStatus=3,
      // �����R�O�B�w�L��
      // Internal command, already expired
      // Return=true/false
  Sys_EnableAdvertisement=4,
      // �����R�O�B�w�L��
      // Internal command, already expired
      // Return=true/false
  Sys_SystemRepair=5,
  Sys_SystemTerminate,
  Sys_GetRecordingStatus,
      // �]�i�H�ϥΡGGetTextSetting()�N��
      // Also can use the function: GetTextSetting() instead
      // p2=physical channel index, Return=RecordingMode
  Sys_SetRecordingStatus,
      // �]�i�H�ϥΡGSetTextSetting()�N��
      // Also can use the function: SetTextSetting() instead
      // p2=physical channel index, p3=RecordingMode, Return=true/false
  Sys_GetRecordingStatus_ViaServer,
      // p2=logical channel index, Return=RecordingMode
  Sys_SetRecordingStatus_ViaServer,
      // p2=logical channel index, p3=RecordingMode, Return=true/false
  Sys_DeleteAllFiles,
      // p2=channel_n, Return=actually deleted count
  Sys_ChangeVideoSetup_not_used_already=12,
  Sys_GetNodeCount=13,
      // RMS�M�ΡA���o�q���`�I�ƶq�]DVR�|�Ǧ^1�^
      // Used primary in RMS, to get the node count (DVR will return 1)
  Sys_GetMaxDVRChannel,
      // RMS�M�ΡA���o�C�Ӹ`�I�ҨϥΪ��v���q�D�ƶq�]�w�]�Ȭ�16�ADVR�|�Ǧ^16�^
      // Used primary in RMS, to get the channel count per node (default is 16,
      // DVR will return 16)
  Sys_GetNodeStatus=15,
      // RMS�M�ΡA���o�q���`�I���A, p2=DVR node index�]�w�L�ɡ^
      // Already expired
  Sys_ChangeVideoSetup_ViaServer_not_used_already=16,
  Sys_RecallSystemDefault=17,
  Sys_FocusView,
      // p2=view index
  Sys_NotifySendAudio=19,
      // �����R�O
      // Internal command
      // p2=sample rate, p3=codec type, p4=Playback Audio Channel
  Sys_QueryConnectionCount,
      // p2=track mask, Return=Connection counts (at least) having this track mask
  Sys_QueryTimeBaseRecycle=21,
      // RMS�M�ΡA�d�ߥثe�O�_�w�g�b����"�ھڮɶ��`�����v"�ʧ@�]�ثe������ʽ�^
      // Experimental command
      // Return=true(time base recycle is processing)/false
  Sys_TriggerTimeBaseRecycle=22,
      // RMS�M�ΡA��ʰ���ھڮɶ��`�����v�ʧ@�]�ثe������ʽ�^
      // Experimental command
      // p2=trigger flag, Return=always true
  Sys_GetPhysicalChannelCount=23,
      // RMS/RMC: physical channel count=0
      // DVR/Hybrid DVR: Real live video channel count from capture card
  Sys_GetNetworkStatus,
      // p2=0: Service connection count, 1: Router connection count
      // Return=Connection count
  Sys_WatchDogCommand,
      // p2=0: �]�wwatch dog���w��۰ʰ����Ҧ��]driver�h�|�۰ʰ����@�~�t�άO�_���A�_�h
      //       �N�|Ĳ�o�t�έ��}���^
      //       Set the watch dog mode to the "Hardware auto detect mode" (The driver will
      //       auto detect if the OS already down, and reboot the system)
      //    1: �]�wwatch dog���n���ʼҦ��A�åB���mwatch dog timer
      //       Set the watch dog mode to the "Software manual mode", and reset the watchdog timer
      //    2: �]�wwatch dog��timer�ɶ�����, p3=timer duration(�i�H���G1~255��)
      //       Set the timer time of the watch dog, p3=timer duration(Maybe: 1~255 second)
      // Return=true/false(If the function not supported)
  Sys_CancelAlarm,
      // ��������DVR���D����ĵ�����A�]2011/06/08�}�l�䴩�^
      // Cancel the alarm status of the remote station

  Sys_CheckAlive=100,
      // �ϥη|����^�Ȫ����V�ˬd�覡�]�����R�O�^
      // Internal command
  Sys_Unknown=10000
};

#define TrackMessageMask 0x01
#define TrackEventMask 0x02
#define TrackGPSMask 0x04
#define TrackBillingMask 0x08
#define TrackAlarmMask 0x10
#define TrackDigitalIOActionMask 0x20
#define TrackSMFEventMask 0x40    // p4=channel filter(-1 for no filter) 

enum RecordingMode {
  RM_Continuous=0,
  RM_ByMD,    // By Motion Detection
  RM_Unknown
};

#define DRF_CheckAlreadyExist 0x01    // �ˬd�n�ƻs���ɮ׬O�_�w�g�s�b

enum ExtendedCommandType {
  Ex_GetLastImage=0,
      // p2=physical channel index
      // �Ǧ^�ȡGResult=true/false, Data1=vf_status, Data2=width, Data3=height,
      //         Time=current time, Message=AM_JPEG
      // �����G���R�O�i�Ǧ^���w��v���Y�ɵ��T���̫�@�i�v��
  Ex_GetLastImage_ViaServer,
      // p2=logical channel index,
      // �Ǧ^�ȡGResult=true/false, Data1=vf_status, Data2=width, Data3=height,
      //         Time=current time, Message=AM_JPEG
  Ex_DuplicateRemoteFile,
      // p2=new local channel index(-1=not specify)
      // p3=group id(-1=not specify)
      // p4=flags
      // text_info_1=remote RMS's IP:Port number
      // text_info_2=remote RMS's file (full path and filename)
      // �Ǧ^�ȡGResult�]�ثe�û��Ǧ^true�^

  Ex_CommunityCheckLogin,
      // text_info_1=user name, text_info_2=plain password
      // �Ǧ^�ȡGResult=true/false
      //         Data1=server index, Data2=node index, Data3=privilege level
      // �����G��ϥΪ̤��s�b�αK�X���~�ɶǦ^�Gfalse
  Ex_CommunityQueryUser,
      // text_info_1=user name
      // �Ǧ^�ȡGResult=true/false
      //         Data1=access handle, Data2=privilege level, Data3=server index, Data4=node index
      //         Time=last login time (future)
      //         Message=user profile (Message.Type1=AM_Message, Message.DataPtr1=user profile text)
      // �����G��ϥΪ̤��s�b�ɶǦ^�Gfalse
      //       ��ϥΪ̤w�n�J�]�b�u�W�^�hData4������NULL
  Ex_CommunityModifyUser,
      // text_info_1=user name, text_info_2=user profile text
      // �Ǧ^�ȡGResult=true/false
      // �����G��ϥΪ̤��s�b�Ψ�L���~�ɶǦ^�Gfalse
  Ex_CommunityDestroyClient,
      // p2=target access handle
      // �Ǧ^�ȡGResult=true/false
      // �����G���R�O�i�H�j��server���_access handle�ҫ��w��remote client�s�u
  Ex_CommunityCreateUser,
      // text_info_1=user name, text_info_2=plain password
      // �Ǧ^�ȡGResult=true/false
      // �����G��ϥΪ̤w�s�b�Ψ�L���~�ɶǦ^�Gfalse
  Ex_CommunityDeleteUser,
      // text_info_1=user name
      // �Ǧ^�ȡGResult=true/false
      // �����G��ϥΪ̤��s�b�Ψ�L���~�ɶǦ^�Gfalse

  Ex_Unknown=10000
};

enum AdvMessageType {
  AM_Message,    // �`�N�GDataPtr1�x�s�r�ꤺ�e
  AM_Event,    // �`�N�GDataPtr1�x�s�r�ꤺ�e
  AM_Billing,    // �p�ɭp�q��T�]DataPtr1�̭���mBillingInfo��Ƶ��c�^
  AM_Alarm,    // ĵ����T�]DataPtr1�̭���mAlarmObject��Ƶ��c�^
  AM_DigitalIOAction,    // Digital I/O�ʧ@��T�]DataPtr1�̭���mDigitalIOAction��Ƶ��c�^
  AM_JPEG,
  AM_SMFEvent,
  AM_GPSMessage,

  AM_Unknown=10000
};

// UDWord NA_ExtendedCommand( unsigned int handle, int p1, UDWord p2, UDWord p3, UDWord p4, C_PortableTime time, const char *text_info_1, const char *text_info_2, UDWord *data1_ptr, UDWord *data2_ptr, UDWord *data3_ptr, UDWord *data4_ptr, C_PortableTime *time_ptr, char *path_filename_buf_ptr, int pfn_buf_size, C_PortableTime *am_time_ptr, int *am_type1_ptr, int *am_type2_ptr, UDWord *am_ext_type1_ptr, UDWord *am_ext_type2_ptr, UByte *data_buf1_ptr, UByte *data_buf2_ptr, int data_buf1_size, int data_buf2_size, int *data_size1_ptr, int *data_size2_ptr )
#define NA_CommunityCheckLogin(handle, req_time, username, password) NA_ExtendedCommand(handle, Ex_CommunityCheckLogin, 0, 0, 0, req_time, username, password, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)
#define NA_CommunityQueryUser(handle, req_time, username, access, priv, ser_idx, node_idx, last_login_time, am_type, am_data, am_data_len, am_data_res_len) NA_ExtendedCommand(handle, Ex_CommunityQueryUser, 0, 0, 0, req_time, username, NULL, access, priv, ser_idx, node_idx, last_login_time, NULL, 0,0, am_type, 0,0,0,am_data,0,am_data_len,0,am_data_res_len,0)
#define NA_CommunityCreateUser(handle, req_time, username, password) NA_ExtendedCommand(handle, Ex_CommunityCreateUser, 0, 0, 0, req_time, username, password, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)
#define NA_CommunityDeleteUser(handle, req_time, username) NA_ExtendedCommand(handle, Ex_CommunityDeleteUser, 0, 0, 0, req_time, username, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)
#define NA_CommunityModifyUser(handle, req_time, username, profile, len_profile) NA_ExtendedCommand(handle, Ex_CommunityModifyUser, 0, 0, 0, req_time, username, profile, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)

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

#define MAX_USER_PROFILE_LEN sizeof(struct user_info)
