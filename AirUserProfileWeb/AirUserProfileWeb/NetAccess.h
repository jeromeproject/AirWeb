/*
Filename: NetAccess.h
Purpose:
Description:
Notice:
*/

#define HF_NetAccess_h


//#define NetAccessStaticLibrary    // 若不使用DLL的方式連結程式庫

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NETACCESS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NETACCESS_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#if !defined( UnixPlatform ) && !defined( NetAccessStaticLibrary )
  #ifdef NETACCESS_EXPORTS
#define NETACCESS_API extern "C" __declspec(dllexport)
  #else
#define NETACCESS_API extern "C" __declspec(dllimport)
  #endif
//#define NA_CALLING_CONVENTION __cdecl
#define NA_CALLING_CONVENTION __stdcall
#else
#define NETACCESS_API
#define NA_CALLING_CONVENTION
#endif


// << 基本型態定義 >>

#ifndef PreventBCBConflicts
  #ifndef Byte
typedef char Byte;
  #endif
#endif

#ifndef UByte
typedef unsigned char UByte;
#endif

#ifndef PreventBCBConflicts
  #ifndef Word
typedef short Word;
  #endif
#endif

#ifndef UWord
typedef unsigned short UWord;
#endif

#ifndef PreventBCBConflicts
  #ifndef DWord
typedef int DWord;
  #endif
#endif

#ifndef UDWord
typedef unsigned int UDWord;
#endif


#ifndef MediaFormat_Defined
enum MediaFormat {
// << 影像部分 >>
  MF_RGB15=0,
  MF_RGB24=1,
  MF_YCbCr422=2,    // Packed格式, YUY2
  MF_YCbCr411=3,    // Planar格式
  MF_YCbCr420=4,    // Planar格式
  MF_JPEG=5,
  MF_VCM=6,    // 所有的VCM codecs
// << 聲音部分 >>
  MF_ByteAudio=7,    // Sample resolution等於8 bits
  MF_WordAudio=8,    // Sample resolution等於16 bits
  MF_ADPCM=9,
  MF_MP3=10,

  MF_YCbCr444=20,    // Packed格式
  MF_RGB32=21,
  MF_RGB16=22,    // BGR656格式

  MF_Unknown=1000
};

#endif


// << 資料結構定義 >>

enum MemoryTypeID {
  MT_FRInfo=0,    // 例如：由NA_GetFileInfo()所傳回的記憶區塊
  MT_FileListItem,    // 例如：由NA_GetFileList()所傳回的記憶區塊
  MT_SMFEvent,    // 例如：由NA_GetEventList()所傳回的記憶區塊

  MT_Unknown=10000
};


struct C_PortableTime {
  DWord Year;    // Years since 1900
  DWord Mon;    // [0, 11]
  DWord Day;    // [1, 31]
  DWord Hour;    // [0, 23]
  DWord Min;    // [0, 59]
  DWord Sec;    // [0, 59]
  DWord MilliSec;    // [0, 999]
  DWord WDay;    // Days since Sunday - [0, 6]
};


struct C_GeneralCommandRV {
  bool Result;    // 表示命令是否正確執行成功...
  UDWord Data1, Data2, Data3, Data4;
  C_PortableTime Time;
  char PathFilename[256];
};


struct C_NComputerInfo {
  UDWord ProtocolId;
  char KernelVersionInfo[64], APVersionInfo[64];    // 軟體版本資訊
  char Name[64], Desc[256];    // 電腦名稱與描述
  UByte MAC[8];    // 主機的MAC位址（最後面兩個byte是為了填塞用）
  UDWord VersionData, FuncAvailable;
};


struct C_NRuntimeInfo {
  UDWord AlarmStatus;    // 若等於1的話則表示警報已經觸發
  int VideoChannelCount;    // DVR主機的影像通道數
  UByte ChannelStatus[32];    // 根據RIC_xx做判斷（目前只預留32個通道的資料）
  UByte HDDStatus[8];
      // 若不等於0，則表示硬碟不能使用，請根據HS_xx做判斷（目前只預留8顆硬碟的資料）
  int DICount, DOCount;
  unsigned int TotalFreeKB, TotalCapacityKB;
  int TotalFileCount;
  UByte MirrorHDDGeneralFlags[8];
  UByte MirrorHDDStatus[8*2];
//#pragma pack(4)
//  __int64 TotalFreeByte, TotalCapacityByte;
//#pragma pack()
};


#ifndef GPS_SatelliteSignalNormal
#define GPS_SatelliteSignalNormal 1
#endif

struct C_GPSMessage {
  UDWord Status;
    // [0~15]=Bit mask
    // [16:25]=Node index（RMS才會填資料）(0~1023)
    // [26:31]=目前找到幾顆衛星(0~63)
  DWord Latitude, Longitude;    // 乘以10000(N=+, S=-), 乘以10000(E=+, W=-)
  Word Speed, Course;    // 乘以10
};


#ifndef AlarmType_DECLARED
enum AlarmType {
  AT_Emergency=0,    // 緊急狀況（手動觸發警報）
  AT_VideoLoss,    // 攝影機訊號中斷：IntegerInfo=Camera index
  AT_OverRecycle,    // 未錄滿n天以上影像資料即發生容量不足（即將過時）
  AT_DiskError,    // 偵測到主硬碟錯誤：IntegerInfo=Path index、StringInfo=錯誤原因
  AT_BackupStorageError,    // 偵測到備份磁碟機錯誤（已過時）
  AT_BackupStorageFull,    // 偵測到備份儲存空間不足（已過時）
  AT_DigitalInputTriggered,    // Digital input觸發：IntegerInfo=DI index
  AT_PowerFailure,    // UPS監控軟體通知電源失效（已過時）
  AT_PeripherialDevice,
      // 周邊設備：反無線盜錄截斷裝置：IntegerInfo=0、StringInfo=訊號頻帶
      //           UPS裝置：IntegerInfo=1
      //（目前沒有相對應的wave檔）
  AT_SystemDiagnosis,
      // 系統診斷報告：檔案系統錯誤：IntegerInfo=0
      //               視訊擷取裝置失效：IntegerInfo=1
      //（目前沒有相對應wave的檔）
  AT_MirrorDiskError,
      // 偵測到Mirror系統的硬碟錯誤：IntegerInfo=disk index (順序：第1個mirror set的主硬碟、
      //     第1個mirror set的次硬碟、第2個mirror set的主硬碟、第2個mirror set的次硬碟...)
      //（目前沒有相對應wave的檔）

  AT_Unknown=10000
};
#endif


struct C_AlarmObject {
  AlarmType Type;
  int IntegerInfo;    // 整數型態的額外資訊
  char StringInfo[256];    // 字串型態的額外資訊
  UDWord Source;    // Bit 0=Direct alarm(RMS side), Bit 1=Remote alarm(DVR side)
  int Index;    // DVR node index (when is indirect alarm)
};


#ifndef SMFEventType_DECLARED
enum SMFEventType {
  ET_VideoLoss=0,    // Info=""
  ET_MotionDetected,    // Info=""
  ET_BeginOfTransaction,    // 插卡之後，Info=卡號字串
  ET_EndOfTransaction,    // 列印明細之後，Info=卡號字串
  ET_DigitalInputTriggered,    // Info=字串
  ET_VideoRecovery,    // Info=""
  ET_GPSData,    // Info=GPSMessage結構
  ET_GSensorCoord,    // Info=GSensorData結構
  ET_DigitalInputSuppressed,    // Info=字串
  ET_POSInformation,    // Info=POS資訊字串
  ET_AlarmTriggered,    // Info=""，目前只用在View的啟動事件聲響用途！

  ET_CameraMovement,    // Info="" （實驗性質）

  ET_LicensePlateNumber,    // Info=車號字串
  ET_UserDefinedBookmark,    // Info=註記字串

  ET_DigitalDevice,    // Info(8 DWords)=device type, digital number 1, error code...

//  ET_MotionStopped,

  ET_Unknown=10000    // 或者表示此事件已經被標記為："已刪除"
};
#endif

struct C_SMFEvent {
  int Index;
  SMFEventType Type;
  char Info[32];    // 也可能是binary模式...
};


#ifndef FRI_Abnormal
#define FRI_Abnormal 0x01
    // 表示檔案可能因為不正常的關檔動作或斷電而部分損毀
#endif

struct C_FRInfo {
  char PathFilename[256-4];
  UDWord Attrib;
  char ComputerName[64-4];
  int FrameCount;
  int ChannelN;
  C_PortableTime FFTime;    // First Frame Time
  int FrameInterval;    // Video Frame Interval
  int VSLength, FileLength;    // Video stream的長度，檔案大小（單位為byte）
};


#ifndef BillingType_DECLARED
// 注意：若是轉送的事件（擁有SigInfo欄位：BT_ServiceConnection、BT_FileDeleted、
//       BT_DownloadCompleted、BT_FileCorrupted）則Status欄位最高位元會被設定為1
enum BillingType {
  BT_ChannelStatus,
      // Index=Channel index(從零開始), Status: Bit 0=Video recovery, Bit 1=Video loss
  BT_DVRConnection,
      // Index=DVR index, Status: Bit 0=Connected, Bit 1=Disconnected
      // RMS專用（RMS才會產生）
  BT_ServiceConnection,
      // SigInfo, Download=Downloaded bytes, Upload=Uploaded bytes
      // StartTime=Connection time, Duration=Connected duration(單位為毫秒)
  BT_MotionStopped,
      // Index=Channel index, Count=Motion frame count, Size=Motion frame size
      // StartTime=Motion start time, Duration=Motion duration(單位為毫秒)
      // RMS專用（RMS才會產生）
  BT_FileRecycled,
      // Index=Channel index (DVR side=physical channel index, RMS side=logical channel index)
      // Status: Bit 0=Direct recycle(RMS side), Bit 1=Remote recycle(DVR side)
      // Count: Path index
      // Size: File size of the recycled file
      // PathFilename=Full path and filename of recycled file
  BT_FileDeleted,
      // Index=Channel index, SigInfo
      // PathFilename=Full path and filename of deleted file
  BT_MACChanged,
      // Index=DVR index, PathFilename=MAC address
  BT_ImageRecovery,
      // Index=DVR index, Size=File size, PathFilename=Recovered video file
      // RMS專用
  BT_VideoAccumulation,
      // Index=Channel index, Count=Accumulated frame count, Size=Accumulated frame size
  BT_InitialChannelStatus,
      // Index=Channel index(從零開始), Status: Bit 0=Video recovery, Bit 1=Video loss
  BT_DownloadCompleted,
      // Index=Channel index(從零開始), SigInfo,
      // Count=Downloading duration(單位為毫秒), Size=Download size
      // StartTime=Downloaded file start time,
      // Duration=Downloaded file time length(單位為毫秒)
  BT_FileCorrupted,
      // Index=Channel index (DVR side=physical channel index, RMS side=logical channel index)
      // Status: Bit 0=Direct corrupted(RMS side), Bit 1=Remote corrupted(DVR side)
      // Count: Path index
      // PathFilename=Full path and filename of recycled file
  BT_MotionStart,
      // Index=Channel index
      // StartTime=Motion start time
      // RMS專用（RMS才會產生）
  BT_ReportVideoSection,
      // Index=Channel index
      // StartTime=Video section start time
      // Duration=Video section time length (unit is second)
      // PathFilename=Comments

  BT_Unknown=10000
};
#endif


struct C_RegSignatureInfo {
  int Type;
  UDWord Data;
  char Signature[64];
};


// 包含全部可能使用的欄位...
struct C_BillingInfo {
  BillingType Type;
  int Index;
  UDWord Status;
  C_RegSignatureInfo SigInfo;
  int Download, Upload;
  int Count, Size;
  C_PortableTime StartTime;
  int Duration;
  char PathFilename[256];
};


struct C_DigitalIOAction {
  UDWord DIData, DITriggerMask, DISuppressMask;
  UDWord DOData, DOTriggerMask, DOSuppressMask;
  UDWord Source;
      // Bit 0=1: Direct action（RMS一開始收到DVR端送過來的事件資料）
      // Bit 1=1: Remote action（RMS轉換過的事件資料）
  int Index;    // DVR node index (when is indirect action)
};


#ifndef FLA_Dir
#define FLA_Dir 0040000
    // 表示是子目錄，"必須"與_S_IFDIR定義相同（注意：bit 12~15是檔案型態的部分）
#endif

#ifndef FLA_Regular
#define FLA_Regular 0100000
    // 表示是regular file，"必須"與_S_IFREG定義相同（注意：bit 12~15是檔案型態的部分）
#endif

struct C_FileListItem {
  char PFN[256];    // 包含路徑與檔名
  int Size;    // 單位為：byte
  UDWord FileAttrib, ExtAttrib;    // , 目前沒用到
  C_PortableTime Time;    // 檔案建立時間
};


struct C_SMFVideoHeader {
  int Width, Height;
  MediaFormat Format;
  UDWord Handler;
  char LocationName[64];    // 播放時可能浮貼在畫面左上角...
  C_PortableTime FFTime;    // First Frame Time
  int FrameInterval;    // 每個frame之間的時間間隔（以毫秒為單位，預設值為33）
  int Length, FrameCount;    // Stream的長度，"實際"frame的數目
};


#ifndef RCFM_WithServerNodeIndex
#define RCFM_WithServerNodeIndex 0x01    // P1/Data1=Server Index, P2/Data2=Node Index
#endif

// << 函數原型定義 >>

NETACCESS_API unsigned int NA_CALLING_CONVENTION NA_CreateObject( unsigned int option );
NETACCESS_API void NA_CALLING_CONVENTION NA_DestroyObject( unsigned int handle );
NETACCESS_API void NA_CALLING_CONVENTION NA_ReleaseMemory( MemoryTypeID mt_id, void *mem_ptr );

NETACCESS_API bool NA_CALLING_CONVENTION NA_GetBillingInfo( unsigned int handle,
    C_BillingInfo *bi_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetAlarmObject( unsigned int handle,
    C_AlarmObject *obj_ptr, C_PortableTime *time_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetDigitalIOActionObject( unsigned int handle,
    C_DigitalIOAction *obj_ptr, C_PortableTime *time_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetSMFEvent( unsigned int handle,
    C_SMFEvent *obj_ptr, int *channel_ptr, C_PortableTime *time_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetGPSMessage( unsigned int handle,
    C_GPSMessage *msg_ptr, C_PortableTime *time_ptr );

NETACCESS_API bool NA_CALLING_CONVENTION NA_IsConnecting( unsigned int handle );
NETACCESS_API bool NA_CALLING_CONVENTION NA_IsSafeConnected( unsigned int handle );
NETACCESS_API bool NA_CALLING_CONVENTION NA_IsSafeDisconnected( unsigned int handle );
NETACCESS_API bool NA_CALLING_CONVENTION NA_Connect( unsigned int handle,
    const char *address, int *logined_pl_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_Disconnect( unsigned int handle );

NETACCESS_API int NA_CALLING_CONVENTION NA_Login( unsigned int handle,
    const char *plain_user_name_ptr, const char *plain_password_ptr );
NETACCESS_API UDWord NA_CALLING_CONVENTION NA_SystemCommand( unsigned int handle,
    int p1, UDWord p2, UDWord p3, UDWord p4 );
NETACCESS_API void NA_CALLING_CONVENTION NA_FastCommand( unsigned int handle,
    int p1, UDWord p2, UDWord p3, UDWord p4, UDWord p5, UDWord p6, UDWord p7, UDWord p8 );
NETACCESS_API void NA_CALLING_CONVENTION NA_GeneralCommand( unsigned int handle,
    int p1, UDWord p2, UDWord p3, UDWord p4, C_PortableTime time, const char *text_info,
    C_GeneralCommandRV *result_ptr );
NETACCESS_API void NA_CALLING_CONVENTION NA_FastGeneralCommand( unsigned int handle,
    int p1, UDWord p2, UDWord p3, UDWord p4, C_PortableTime time, const char *text_info );
NETACCESS_API UDWord NA_CALLING_CONVENTION NA_ExtendedCommand( unsigned int handle,
    int p1, UDWord p2, UDWord p3, UDWord p4, C_PortableTime time,
    const char *text_info_1, const char *text_info_2,
    UDWord *data1_ptr, UDWord *data2_ptr, UDWord *data3_ptr, UDWord *data4_ptr,
    C_PortableTime *time_ptr, char *path_filename_buf_ptr, int pfn_buf_size,
    C_PortableTime *am_time_ptr,
    int *am_type1_ptr, int *am_type2_ptr,
    UDWord *am_ext_type1_ptr, UDWord *am_ext_type2_ptr,
    UByte *data_buf1_ptr, UByte *data_buf2_ptr,
    int data_buf1_size, int data_buf2_size,
    int *data_size1_ptr, int *data_size2_ptr );

NETACCESS_API bool NA_CALLING_CONVENTION NA_ControlPTZ_SendRecvRawData( unsigned int handle,
    int channel_n, const void *send_data_ptr, int send_data_size,
    UByte *recv_data_ptr, int max_recv_data_size, int *recv_data_size_ptr,
    int max_recv_wait_time );

NETACCESS_API bool NA_CALLING_CONVENTION NA_GetTextSetting( unsigned int handle,
    int type1, int type2, UDWord data1, UDWord data2,
    char *setting_buf, int setting_buf_size );
NETACCESS_API bool NA_CALLING_CONVENTION NA_SetTextSetting( unsigned int handle,
    int type1, int type2, UDWord data1, UDWord data2,
    const char *setting_buf );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetIni( unsigned int handle,
    const char *ini_name, char *text_buf_ptr, int text_buf_size );

NETACCESS_API bool NA_CALLING_CONVENTION NA_GetNodeComputerInfo( unsigned int handle,
    int p1, int p2, C_NComputerInfo *info_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetNodeRuntimeInfo( unsigned int handle,
    int p1, int p2, C_NRuntimeInfo *info_ptr );

NETACCESS_API bool NA_CALLING_CONVENTION NA_GetFileInfo( unsigned int handle,
    int channel_n, bool filter_flag,
    C_PortableTime filter_begin, C_PortableTime filter_end,
    C_FRInfo **info_ptr_ptr, int *info_count_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetEventList( unsigned int handle,
    const char *pfn, C_PortableTime *fe_time_ptr, int *ei_ptr,
    C_SMFEvent **event_ptr_ptr, int *event_count_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_DeleteMediaFile( unsigned int handle,
    const char *pfn );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetFileList( unsigned int handle,
    const char *path, C_FileListItem **list_ptr_ptr, int *item_count_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_TransferFile( unsigned int handle,
    const char *path_filename, UDWord attrib, void *data_ptr, int data_size );

NETACCESS_API bool NA_CALLING_CONVENTION NA_DownloadMediaFile( unsigned int handle,
    const char *remote_pfn, const char *local_pfn, UDWord option );
NETACCESS_API bool NA_CALLING_CONVENTION NA_DownloadPartialMediaFile( unsigned int handle,
    const char *remote_pfn, const char *local_pfn,
    C_PortableTime from_time, C_PortableTime to_time );
NETACCESS_API bool NA_CALLING_CONVENTION NA_CancelDownload( unsigned int handle );
NETACCESS_API bool NA_CALLING_CONVENTION NA_QueryDownloadStatus( unsigned int handle,
    bool *last_result_ptr, int *total_bytes_ptr, int *downloaded_bytes_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_AdjustClock( unsigned int handle );

NETACCESS_API bool NA_CALLING_CONVENTION NA_StartVideoChannel( unsigned int handle,
    int channel_n, MediaFormat format, int frame_rate,
    int *width_ptr, int *height_ptr, MediaFormat *format_ptr, UDWord *handler_ptr,
    UDWord *func_available_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_StopVideoChannel( unsigned int handle, int channel_n );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetVideoFrame( unsigned int handle,
    int *width_ptr, int *height_ptr, MediaFormat *format_ptr, unsigned int *status_ptr,
    C_PortableTime *time_ptr, unsigned char *buffer_ptr, int buffer_size, int *bytes_used_ptr );

NETACCESS_API bool NA_CALLING_CONVENTION NA_StartAudioChannel( unsigned int handle,
    int channel_n, MediaFormat format, int *sample_rate_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_StopAudioChannel( unsigned int handle, int channel_n );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetAudioSample( unsigned int handle,
    MediaFormat *format_ptr, unsigned int *status_ptr,
    C_PortableTime *time_ptr, unsigned char *buffer_ptr, int buffer_size, int *bytes_used_ptr );

NETACCESS_API bool NA_CALLING_CONVENTION NA_RegisterComputer( unsigned int handle,
    UDWord status, int port_n, const char *name, const char *desc,
    UDWord func_mask, UDWord p1, UDWord p2, UDWord p3, UDWord p4,
    UDWord *func_mask_ptr, UDWord *data1_ptr, UDWord *data2_ptr, UDWord *data3_ptr, UDWord *data4_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_QueryComputer( unsigned int handle,
    UDWord status, const char *name,
    UDWord *status_ptr, char *ip_buf, int ip_size, int *port_n_ptr, UDWord *object_handle_ptr,
    UDWord *func_mask_ptr, UDWord *p1_ptr, UDWord *p2_ptr, UDWord *p3_ptr, UDWord *p4_ptr,
    C_PortableTime *time_ptr );
NETACCESS_API int NA_CALLING_CONVENTION NA_BroadcastComputerData( unsigned int handle,
    UDWord status, int target_count, const UDWord *target_handle_ptrs,
    int data_size, const UByte *data_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_GetComputerCommuData( unsigned int handle,
    UDWord *source_handle_ptr, UDWord *status_ptr, int *data_size_ptr,
    UByte *data_buf_ptr, int data_buf_size );

NETACCESS_API bool NA_CALLING_CONVENTION NA_LockMediaFile( unsigned int handle,
    const char *pfn, bool need_read_only,
    int *file_size_ptr, C_SMFVideoHeader *v_header_ptr );
NETACCESS_API bool NA_CALLING_CONVENTION NA_UnlockMediaFile( unsigned int handle, const char *pfn );
NETACCESS_API bool NA_CALLING_CONVENTION NA_SeekAndGetVideoFrame( unsigned int handle,
    int start_index, bool seek_forward, UDWord attrib,
    int *index_ptr, int *size_ptr, UDWord *attrib_ptr, UByte *buffer_ptr, int buffer_size,
    int *length_ptr, int *frame_count_ptr );

NETACCESS_API bool NA_CALLING_CONVENTION NA_CancelRemoteAlarm( unsigned int handle );
