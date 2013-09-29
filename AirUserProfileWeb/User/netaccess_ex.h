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
