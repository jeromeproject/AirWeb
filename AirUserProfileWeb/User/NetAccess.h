/*
Filename: NetAccess.h
Purpose:
Description:
Notice:
*/

#define HF_NetAccess_h


//#define NetAccessStaticLibrary    // �Y���ϥ�DLL���覡�s���{���w

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


// << �򥻫��A�w�q >>

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
// << �v������ >>
  MF_RGB15=0,
  MF_RGB24=1,
  MF_YCbCr422=2,    // Packed�榡, YUY2
  MF_YCbCr411=3,    // Planar�榡
  MF_YCbCr420=4,    // Planar�榡
  MF_JPEG=5,
  MF_VCM=6,    // �Ҧ���VCM codecs
// << �n������ >>
  MF_ByteAudio=7,    // Sample resolution����8 bits
  MF_WordAudio=8,    // Sample resolution����16 bits
  MF_ADPCM=9,
  MF_MP3=10,

  MF_YCbCr444=20,    // Packed�榡
  MF_RGB32=21,
  MF_RGB16=22,    // BGR656�榡

  MF_Unknown=1000
};

#endif


// << ��Ƶ��c�w�q >>

enum MemoryTypeID {
  MT_FRInfo=0,    // �Ҧp�G��NA_GetFileInfo()�ҶǦ^���O�а϶�
  MT_FileListItem,    // �Ҧp�G��NA_GetFileList()�ҶǦ^���O�а϶�
  MT_SMFEvent,    // �Ҧp�G��NA_GetEventList()�ҶǦ^���O�а϶�

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
  bool Result;    // ��ܩR�O�O�_���T���榨�\...
  UDWord Data1, Data2, Data3, Data4;
  C_PortableTime Time;
  char PathFilename[256];
};


struct C_NComputerInfo {
  UDWord ProtocolId;
  char KernelVersionInfo[64], APVersionInfo[64];    // �n�骩����T
  char Name[64], Desc[256];    // �q���W�ٻP�y�z
  UByte MAC[8];    // �D����MAC��}�]�̫᭱���byte�O���F���Ρ^
  UDWord VersionData, FuncAvailable;
};


struct C_NRuntimeInfo {
  UDWord AlarmStatus;    // �Y����1���ܫh���ĵ���w�gĲ�o
  int VideoChannelCount;    // DVR�D�����v���q�D��
  UByte ChannelStatus[32];    // �ھ�RIC_xx���P�_�]�ثe�u�w�d32�ӳq�D����ơ^
  UByte HDDStatus[8];
      // �Y������0�A�h��ܵw�Ф���ϥΡA�Юھ�HS_xx���P�_�]�ثe�u�w�d8���w�Ъ���ơ^
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
    // [16:25]=Node index�]RMS�~�|���ơ^(0~1023)
    // [26:31]=�ثe���X���ìP(0~63)
  DWord Latitude, Longitude;    // ���H10000(N=+, S=-), ���H10000(E=+, W=-)
  Word Speed, Course;    // ���H10
};


#ifndef AlarmType_DECLARED
enum AlarmType {
  AT_Emergency=0,    // ��檬�p�]���Ĳ�oĵ���^
  AT_VideoLoss,    // ��v���T�����_�GIntegerInfo=Camera index
  AT_OverRecycle,    // ������n�ѥH�W�v����ƧY�o�ͮe�q�����]�Y�N�L�ɡ^
  AT_DiskError,    // ������D�w�п��~�GIntegerInfo=Path index�BStringInfo=���~��]
  AT_BackupStorageError,    // ������ƥ��Ϻо����~�]�w�L�ɡ^
  AT_BackupStorageFull,    // ������ƥ��x�s�Ŷ������]�w�L�ɡ^
  AT_DigitalInputTriggered,    // Digital inputĲ�o�GIntegerInfo=DI index
  AT_PowerFailure,    // UPS�ʱ��n��q���q�����ġ]�w�L�ɡ^
  AT_PeripherialDevice,
      // �P��]�ơG�ϵL�u�s���I�_�˸m�GIntegerInfo=0�BStringInfo=�T���W�a
      //           UPS�˸m�GIntegerInfo=1
      //�]�ثe�S���۹�����wave�ɡ^
  AT_SystemDiagnosis,
      // �t�ζE�_���i�G�ɮרt�ο��~�GIntegerInfo=0
      //               ���T�^���˸m���ġGIntegerInfo=1
      //�]�ثe�S���۹���wave���ɡ^
  AT_MirrorDiskError,
      // ������Mirror�t�Ϊ��w�п��~�GIntegerInfo=disk index (���ǡG��1��mirror set���D�w�СB
      //     ��1��mirror set�����w�СB��2��mirror set���D�w�СB��2��mirror set�����w��...)
      //�]�ثe�S���۹���wave���ɡ^

  AT_Unknown=10000
};
#endif


struct C_AlarmObject {
  AlarmType Type;
  int IntegerInfo;    // ��ƫ��A���B�~��T
  char StringInfo[256];    // �r�ꫬ�A���B�~��T
  UDWord Source;    // Bit 0=Direct alarm(RMS side), Bit 1=Remote alarm(DVR side)
  int Index;    // DVR node index (when is indirect alarm)
};


#ifndef SMFEventType_DECLARED
enum SMFEventType {
  ET_VideoLoss=0,    // Info=""
  ET_MotionDetected,    // Info=""
  ET_BeginOfTransaction,    // ���d����AInfo=�d���r��
  ET_EndOfTransaction,    // �C�L���Ӥ���AInfo=�d���r��
  ET_DigitalInputTriggered,    // Info=�r��
  ET_VideoRecovery,    // Info=""
  ET_GPSData,    // Info=GPSMessage���c
  ET_GSensorCoord,    // Info=GSensorData���c
  ET_DigitalInputSuppressed,    // Info=�r��
  ET_POSInformation,    // Info=POS��T�r��
  ET_AlarmTriggered,    // Info=""�A�ثe�u�ΦbView���Ұʨƥ��n�T�γ~�I

  ET_CameraMovement,    // Info="" �]����ʽ�^

  ET_LicensePlateNumber,    // Info=�����r��
  ET_UserDefinedBookmark,    // Info=���O�r��

  ET_DigitalDevice,    // Info(8 DWords)=device type, digital number 1, error code...

//  ET_MotionStopped,

  ET_Unknown=10000    // �Ϊ̪�ܦ��ƥ�w�g�Q�аO���G"�w�R��"
};
#endif

struct C_SMFEvent {
  int Index;
  SMFEventType Type;
  char Info[32];    // �]�i��Obinary�Ҧ�...
};


#ifndef FRI_Abnormal
#define FRI_Abnormal 0x01
    // ����ɮץi��]�������`�����ɰʧ@���_�q�ӳ����l��
#endif

struct C_FRInfo {
  char PathFilename[256-4];
  UDWord Attrib;
  char ComputerName[64-4];
  int FrameCount;
  int ChannelN;
  C_PortableTime FFTime;    // First Frame Time
  int FrameInterval;    // Video Frame Interval
  int VSLength, FileLength;    // Video stream�����סA�ɮפj�p�]��쬰byte�^
};


#ifndef BillingType_DECLARED
// �`�N�G�Y�O��e���ƥ�]�֦�SigInfo���GBT_ServiceConnection�BBT_FileDeleted�B
//       BT_DownloadCompleted�BBT_FileCorrupted�^�hStatus���̰��줸�|�Q�]�w��1
enum BillingType {
  BT_ChannelStatus,
      // Index=Channel index(�q�s�}�l), Status: Bit 0=Video recovery, Bit 1=Video loss
  BT_DVRConnection,
      // Index=DVR index, Status: Bit 0=Connected, Bit 1=Disconnected
      // RMS�M�Ρ]RMS�~�|���͡^
  BT_ServiceConnection,
      // SigInfo, Download=Downloaded bytes, Upload=Uploaded bytes
      // StartTime=Connection time, Duration=Connected duration(��쬰�@��)
  BT_MotionStopped,
      // Index=Channel index, Count=Motion frame count, Size=Motion frame size
      // StartTime=Motion start time, Duration=Motion duration(��쬰�@��)
      // RMS�M�Ρ]RMS�~�|���͡^
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
      // RMS�M��
  BT_VideoAccumulation,
      // Index=Channel index, Count=Accumulated frame count, Size=Accumulated frame size
  BT_InitialChannelStatus,
      // Index=Channel index(�q�s�}�l), Status: Bit 0=Video recovery, Bit 1=Video loss
  BT_DownloadCompleted,
      // Index=Channel index(�q�s�}�l), SigInfo,
      // Count=Downloading duration(��쬰�@��), Size=Download size
      // StartTime=Downloaded file start time,
      // Duration=Downloaded file time length(��쬰�@��)
  BT_FileCorrupted,
      // Index=Channel index (DVR side=physical channel index, RMS side=logical channel index)
      // Status: Bit 0=Direct corrupted(RMS side), Bit 1=Remote corrupted(DVR side)
      // Count: Path index
      // PathFilename=Full path and filename of recycled file
  BT_MotionStart,
      // Index=Channel index
      // StartTime=Motion start time
      // RMS�M�Ρ]RMS�~�|���͡^
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


// �]�t�����i��ϥΪ����...
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
      // Bit 0=1: Direct action�]RMS�@�}�l����DVR�ݰe�L�Ӫ��ƥ��ơ^
      // Bit 1=1: Remote action�]RMS�ഫ�L���ƥ��ơ^
  int Index;    // DVR node index (when is indirect action)
};


#ifndef FLA_Dir
#define FLA_Dir 0040000
    // ��ܬO�l�ؿ��A"����"�P_S_IFDIR�w�q�ۦP�]�`�N�Gbit 12~15�O�ɮ׫��A�������^
#endif

#ifndef FLA_Regular
#define FLA_Regular 0100000
    // ��ܬOregular file�A"����"�P_S_IFREG�w�q�ۦP�]�`�N�Gbit 12~15�O�ɮ׫��A�������^
#endif

struct C_FileListItem {
  char PFN[256];    // �]�t���|�P�ɦW
  int Size;    // ��쬰�Gbyte
  UDWord FileAttrib, ExtAttrib;    // , �ثe�S�Ψ�
  C_PortableTime Time;    // �ɮ׫إ߮ɶ�
};


struct C_SMFVideoHeader {
  int Width, Height;
  MediaFormat Format;
  UDWord Handler;
  char LocationName[64];    // ����ɥi��B�K�b�e�����W��...
  C_PortableTime FFTime;    // First Frame Time
  int FrameInterval;    // �C��frame�������ɶ����j�]�H�@�����A�w�]�Ȭ�33�^
  int Length, FrameCount;    // Stream�����סA"���"frame���ƥ�
};


#ifndef RCFM_WithServerNodeIndex
#define RCFM_WithServerNodeIndex 0x01    // P1/Data1=Server Index, P2/Data2=Node Index
#endif

// << ��ƭ쫬�w�q >>

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
