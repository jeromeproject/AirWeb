enum SysCommandType {
  Sys_Reboot=0,
      // 遠端系統重開機
      // Reboot the remote station
  Sys_ShutDown,
      // 執行遠端電腦關機
      // Shut down the remote station
  Sys_TrackMessage,
      // p2=track mask, p3=option flags(reserved), p4=command data, Return=true/false
  Sys_GetAdvertiseStatus=3,
      // 內部命令、已過時
      // Internal command, already expired
      // Return=true/false
  Sys_EnableAdvertisement=4,
      // 內部命令、已過時
      // Internal command, already expired
      // Return=true/false
  Sys_SystemRepair=5,
  Sys_SystemTerminate,
  Sys_GetRecordingStatus,
      // 也可以使用：GetTextSetting()代替
      // Also can use the function: GetTextSetting() instead
      // p2=physical channel index, Return=RecordingMode
  Sys_SetRecordingStatus,
      // 也可以使用：SetTextSetting()代替
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
      // RMS專用，取得電腦節點數量（DVR會傳回1）
      // Used primary in RMS, to get the node count (DVR will return 1)
  Sys_GetMaxDVRChannel,
      // RMS專用，取得每個節點所使用的影像通道數量（預設值為16，DVR會傳回16）
      // Used primary in RMS, to get the channel count per node (default is 16,
      // DVR will return 16)
  Sys_GetNodeStatus=15,
      // RMS專用，取得電腦節點狀態, p2=DVR node index（已過時）
      // Already expired
  Sys_ChangeVideoSetup_ViaServer_not_used_already=16,
  Sys_RecallSystemDefault=17,
  Sys_FocusView,
      // p2=view index
  Sys_NotifySendAudio=19,
      // 內部命令
      // Internal command
      // p2=sample rate, p3=codec type, p4=Playback Audio Channel
  Sys_QueryConnectionCount,
      // p2=track mask, Return=Connection counts (at least) having this track mask
  Sys_QueryTimeBaseRecycle=21,
      // RMS專用，查詢目前是否已經在執行"根據時間循環錄影"動作（目前為實驗性質）
      // Experimental command
      // Return=true(time base recycle is processing)/false
  Sys_TriggerTimeBaseRecycle=22,
      // RMS專用，手動執行根據時間循環錄影動作（目前為實驗性質）
      // Experimental command
      // p2=trigger flag, Return=always true
  Sys_GetPhysicalChannelCount=23,
      // RMS/RMC: physical channel count=0
      // DVR/Hybrid DVR: Real live video channel count from capture card
  Sys_GetNetworkStatus,
      // p2=0: Service connection count, 1: Router connection count
      // Return=Connection count
  Sys_WatchDogCommand,
      // p2=0: 設定watch dog為硬體自動偵測模式（driver層會自動偵測作業系統是否當掉，否則
      //       就會觸發系統重開機）
      //       Set the watch dog mode to the "Hardware auto detect mode" (The driver will
      //       auto detect if the OS already down, and reboot the system)
      //    1: 設定watch dog為軟體手動模式，並且重置watch dog timer
      //       Set the watch dog mode to the "Software manual mode", and reset the watchdog timer
      //    2: 設定watch dog的timer時間長度, p3=timer duration(可以為：1~255秒)
      //       Set the timer time of the watch dog, p3=timer duration(Maybe: 1~255 second)
      // Return=true/false(If the function not supported)
  Sys_CancelAlarm,
      // 取消遠端DVR等主機的警報狀態（2011/06/08開始支援）
      // Cancel the alarm status of the remote station

  Sys_CheckAlive=100,
      // 使用會有返回值的雙向檢查方式（內部命令）
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

#define DRF_CheckAlreadyExist 0x01    // 檢查要複製的檔案是否已經存在

enum ExtendedCommandType {
  Ex_GetLastImage=0,
      // p2=physical channel index
      // 傳回值：Result=true/false, Data1=vf_status, Data2=width, Data3=height,
      //         Time=current time, Message=AM_JPEG
      // 說明：本命令可傳回指定攝影機即時視訊的最後一張影像
  Ex_GetLastImage_ViaServer,
      // p2=logical channel index,
      // 傳回值：Result=true/false, Data1=vf_status, Data2=width, Data3=height,
      //         Time=current time, Message=AM_JPEG
  Ex_DuplicateRemoteFile,
      // p2=new local channel index(-1=not specify)
      // p3=group id(-1=not specify)
      // p4=flags
      // text_info_1=remote RMS's IP:Port number
      // text_info_2=remote RMS's file (full path and filename)
      // 傳回值：Result（目前永遠傳回true）

  Ex_CommunityCheckLogin,
      // text_info_1=user name, text_info_2=plain password
      // 傳回值：Result=true/false
      //         Data1=server index, Data2=node index, Data3=privilege level
      // 說明：當使用者不存在或密碼錯誤時傳回：false
  Ex_CommunityQueryUser,
  //  Ex_CommunityGetUserProfile
      // text_info_1=user name
      // 傳回值：Result=true/false
      //         Data1=access handle, Data2=privilege level, Data3=server index, Data4=node index
      //         Time=last login time (future)
      //         Message=user profile (Message.Type1=AM_Message, Message.DataPtr1=user profile text)
      // 說明：當使用者不存在時傳回：false
      //       當使用者已登入（在線上）則Data4不等於NULL
  Ex_CommunityModifyUser,
      // text_info_1=user name, text_info_2=user profile text
      // 傳回值：Result=true/false
      // 說明：當使用者不存在或其他錯誤時傳回：false
  Ex_CommunityDestroyClient,
      // p2=target access handle
      // 傳回值：Result=true/false
      // 說明：本命令可以強迫server中斷access handle所指定的remote client連線
  Ex_CommunityCreateUser,
      // text_info_1=user name, text_info_2=plain password
      // 傳回值：Result=true/false
      // 說明：當使用者已存在或其他錯誤時傳回：false
  Ex_CommunityDeleteUser,
      // text_info_1=user name
      // 傳回值：Result=true/false
      // 說明：當使用者不存在或其他錯誤時傳回：false
  Ex_CommunityListUser,
      // text_info_1=query string
      // 傳回值：Result=true
      //         Message=user name list (Message.Type1=AM_Message, Message.DataPtr1=user name list)
      // 說明：傳回根據查詢字串過濾過後的使用者列表（每個使用者名稱使用'\n'隔開）
  Ex_CommunityQueryUserInfo,
      // text_info_1=user/device name
      // 傳回值：Result=true/false
      //         Message=user information (Message.Type1=AM_Message, Message.DataPtr1=user information text)
      // 說明：當使用者不存在時傳回：false

  Ex_GetDeviceInitialSetting,
      // text_info_1=device id
      // 傳回值：Result=true/false
      //         Message=setting data (Message.Type1=AM_Message, Message.DataPtr1=ini setting text)
  Ex_SetDeviceInitialSetting,
      // text_info_1=device id
      // text_info_2=setting data
      // 傳回值：Result=true/false

  Ex_CommunityListDevice,
      // text_info_1=query string
      // 傳回值：Result=true
      //         Message=device name list (Message.Type1=AM_Message, Message.DataPtr1=device name list)
      // 說明：傳回根據查詢字串過濾過後的裝置列表（每個裝置名稱使用'\n'隔開）
  Ex_CommunityRemoveDeviceRelationship,
      // text_info_1=device id
      // 傳回值：Result=true
      // 說明：本功能會刪除所有使用者好友列表裡面包含此裝置的關連資料內容


  Ex_Unknown=10000
};

enum AdvMessageType {
  AM_Message,    // 注意：DataPtr1儲存字串內容
  AM_Event,    // 注意：DataPtr1儲存字串內容
  AM_Billing,    // 計時計量資訊（DataPtr1裡面放置BillingInfo資料結構）
  AM_Alarm,    // 警報資訊（DataPtr1裡面放置AlarmObject資料結構）
  AM_DigitalIOAction,    // Digital I/O動作資訊（DataPtr1裡面放置DigitalIOAction資料結構）
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

#define NA_CommunityListUser(handle, req_time, query_string, am_type, am_data, am_data_len,am_data_res_len) NA_ExtendedCommand(handle, Ex_CommunityListUser, 0, 0, 0, req_time, query_string, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0,0, am_type, 0,0,0,am_data,0,am_data_len,0,am_data_res_len,0)

//
// friend list(device operation)
//

#define NA_CommunityListDevice(handle, req_time, query_string, am_type, am_data, am_data_len,am_data_res_len) NA_ExtendedCommand(handle, Ex_CommunityListDevice, 0, 0, 0, req_time, query_string, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0,0, am_type, 0,0,0,am_data,0,am_data_len,0,am_data_res_len,0)

#define NA_CommunityQueryDevice(handle, req_time, device, am_type, am_data, am_data_len, am_data_res_len) NA_CommunityQueryUser(handle, req_time, device, NULL, NULL, NULL, NULL, NULL, am_type, am_data, am_data_len, am_data_res_len)

// device initial settting: NameServerAddress=rms_0.dynamas.com.tw:17861
#define NA_CommunityCreateDevice(handle, req_time, device, init_setting) NA_CommunityCreateUser(handle, req_time, device, "1234") && \
		NA_ExtendedCommand(handle, Ex_SetDeviceInitialSetting, 0, 0, 0, req_time, device, init_setting, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)

#define NA_CommunityDeleteDevice(handle, req_time, device) \
	 NA_ExtendedCommand(handle, Ex_CommunityDeleteUser, 0, 0, 0, req_time, device, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0) && \
	 NA_ExtendedCommand(handle, Ex_CommunityRemoveDeviceRelationship, 0, 0, 0, req_time, device, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0,0,0,0,0,0,0,0,0,0,0,0)

#define NA_CommunityModifyDevice(handle, req_time, device, profile, len_profile)  NA_CommunityModifyUser(handle, req_time, device, profile, len_profile)


#define NA_SysReboot(handle) NA_SystemCommand(handle, Sys_Reboot, 0, 0, 0)
#define NA_SysGetNodeCount(handle) NA_SystemCommand(handle, Sys_GetNodeCount, 0, 0, 0)

enum SettingType {
  ST_DVR_System=0,    // DVR主機系統設定
  ST_DVR_Channel,    // DVR主機視訊通道設定值，type2=Channel index
  ST_RMS_System,    // 未實作
  ST_RMS_Node,    // RMS伺服器節點設定，type2=Node index
  ST_RMS_Channel,    // RMS伺服器影像通道連結設定，type2=Channel index
  ST_DVR_System_ViaServer,    // type2=Node index
  ST_DVR_Channel_ViaServer,    // type2=Channel index (numbered according to server)
  ST_DVR_Storage,    // DVR主機儲存路徑等設定值（RMS伺服器同樣也有相同設定）
  ST_DVR_Storage_ViaServer,    // type2=Node index
  ST_LANSetting,    // type2: 0=區域網路等設定
  ST_Debug_SystemInfo=100,
      // 傳回文字形式的系統資訊（Debug用途）
      // type2= 0: video server information, 1: frame work information,
      //        2: network status, 3: node connection status(RMS)
  ST_SettingUnknown=10000
};

//bool NA_GetTextSetting( unsigned int handle, int type1, int type2, UDWord data1, UDWord data2, char *setting_buf, int setting_buf_size );
#define NA_GetNodeTextSetting(handle, node_num, setting_buf, setting_buf_size) NA_GetTextSetting(handle, ST_RMS_Node, node_num, 0, 0, setting_buf, setting_buf_size)
// bool NA_SetTextSetting( unsigned int handle, int type1, int type2, UDWord data1, UDWord data2, const char *setting_buf );
/*
handle: 介面程式物件的handle值
type1: 指定的設定值型態
type2、data1、data2: 其他可有可無的的選項參數
setting_buf: 所提供文字設定值資料的緩衝區位址
*/
#define NA_SetNodeTextSetting(handle, node_num, setting_buf) NA_SetTextSetting(handle, ST_RMS_Node, node_num, 0, 0, setting_buf)

/*
ComputerName=RMServer
	ComputerDesc=
EnableVideoLossAlarm=0
EnablePureMotionDetection=0
ConservativeNU=1
SM_BaseSendLifeTime=180
rts_addr_0=localhost:17990
rts_addr_1=
rts_addr_2=
rts_addr_3=
network_port_number=17861
OptimizeJPEGEncode=0
MPEG4EncodeComplexity=2
RecordMPEG4KFInterval=45
NetworkMPEG4KFInterval=60
HD_AutoDeinterlace=0
EnableNSRegister=0
NameServerAddress=
*/
#define NA_GetDVRTextSetting(handel, setting_buf, setting_buf_size) NA_GetTextSetting(handle, ST_DVR_System, 0, 0, 0, setting_buf, setting_buf_size)

/*
netcon -gs dynamas.com.tw:17861 4 17 0 0
CS_17_IsEnable=0
CS_17_StorageSpace=20971520
CS_17_StorageTime=0
CS_17_AdvancedMode=0
CS_17_ReqFormat=6
CS_17_Quality=75
CS_17_KFInterval=60
CS_17_FR1=300
CS_17_FR2=300
CS_17_FR3=100
CS_17_FR4=400
CS_17_FR5=10
CS_17_RequestAudioStream=0

2014/5/18 default value
CS_0_IsEnable=1  ---------> 是否 "啟動（視訊）監控"
CS_0_RequestAudioStream=1  ------> 是否 "要求音訊串流"
*/
#define NA_SetChannelTextSetting(handle, ch_num, setting_buf) NA_SetTextSetting(handle, ST_RMS_Channel, ch_num, 0, 0, setting_buf)


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
	char PlainPassword[64];
	int UserPL;
	char Nickname[64];
	char StatusComment[128];
	char FriendList[14000];
	char BlackList[14000];
	int ServerIndex;
	int NodeIndex;
	char QueryString[5000];
};

struct rms_node_info
{
	int index;
	int IsEnable;	// 1
	int TargetType;	// 0
	int SubTargetType;	//10000
	char IPAddress[128];		//_D_1C7B21594E84
	int PortNumber;		//17860
	int Lookup;			// 0
	int WaitForConnect;	//1
	char UserName[128];		//Admin
	char Password[128];
	int TrackMessage;	//1
	int TrackEvent;	//1
	int TrackGPS;	//1
	int AutoConnect;	//1
	int AutoAdjustTime;	//0
	int AutoImageRecovery;	//0
};

#define MAX_USER_PROFILE_LEN sizeof(struct user_info)
#define MAX_USER_LIST_SIZE 14096
