// -*- coding: gb2312-dos -*-
#ifndef __MV_GIGEVISION_GVSP_H__
#define __MV_GIGEVISION_GVSP_H__


#if MV_GIGEVISION_MAJOR_VERSION == 1

/////////////////////////////////////////////////////GVSP Define ////////////////////////////////////////////
/////////////////////////////////////// GVSP common packet header define ////////////////////////////////////

/*                                All GVSP packets share the same basic header.
 *  0 bit                    7|8                      15|16                     23|24                     31|
 *  +------------+------------+------------+------------+------------+------------+------------+------------+
 *  |                      status                       |                      block_id                     |
 *  ---------------------------------------------------------------------------------------------------------
 *  |        packet_fmt       |                                    packet_id                                |
 *  ---------------------------------------------------------------------------------------------------------
 *  |                                           ......                                                      |
 *  +------------+------------+------------+------------+------------+------------+------------+------------+
 */
typedef struct _GVSP_PACKET_HEADER_
{
    unsigned short  status;                     // 16bits��״̬��
    unsigned short  block_id;                   // 16bits

    union
    {
        struct
        {
            unsigned int    packet_fmt:8;               // 8bits, �ο� GVSP�İ����Ͷ���
            unsigned int    packet_id:24;               // 24bits
        };
        unsigned int    packet_fmt_id;
    };

} GVSP_PACKET_HEADER;


#define GVSP_PACKET_ID_MAX                  0x00FFFFFF

// GVSP�İ����ͣ�Ŀǰ����6��ֵ����������
// For Standard Transmission Mode:
#define GVSP_PACKET_FMT_LEADER              0x01
#define GVSP_PACKET_FMT_TRAILER             0x02
#define GVSP_PACKET_FMT_PAYLOAD_GENERIC     0x03
// For All-in Transmission Mode:
#define GVSP_PACKET_FMT_ALL_IN              0x04


// GVSP ���ĸ�������
typedef enum _GVSP_PACKET_PAYLOAD_TYPE_
{
    GVSP_PT_UNCOMPRESSED_IMAGE          = 0x0001,
    GVSP_PT_RAW_DATA                    = 0x0002,
    GVSP_PT_FILE                        = 0x0003,
    GVSP_PT_CHUNK_DATA                  = 0x0004,
    GVSP_PT_CHUNK_DATA_EX               = 0x0005,

    // [0x8000, 0xffff]�Զ���

} GVSP_PACKET_PAYLOAD_TYPE;


//////////////////////////// Data Leader define ////////////////////////////

// additional info for Image Data Leader Packet
typedef struct _GVSP_IMAGE_DATA_LEADER_
{
    unsigned short  reserved;           // ��0
    unsigned short  payload_type;       // 0x0001
    unsigned int    timestamp_high;
    unsigned int    timestamp_low;
    unsigned int    pixel_format;
    unsigned int    size_x;
    unsigned int    size_y;
    unsigned int    offset_x;
    unsigned int    offset_y;
    unsigned short  padding_x;
    unsigned short  padding_y;
} GVSP_IMAGE_DATA_LEADER;

//////////////////////////// Data Payload define ////////////////////////////


//////////////////////////// Data Trailer define ////////////////////////////

// additional info for Image Data Trailer Packet
typedef struct _GVSP_IMAGE_DATA_TRAILER_
{
    unsigned short  reserved;           // ��0
    unsigned short  payload_type;       // 0x0001
    unsigned int    size_y;             // 32bit
} GVSP_IMAGE_DATA_TRAILER;

#endif  // MV_GIGEVISION_MAJOR_VERSION == 1



#if MV_GIGEVISION_MAJOR_VERSION == 2
/*                                All GVSP packets share the same basic header.
 *  0 bit                    7|8                      15|16                     23|24                     31|
 *  +------------+------------+------------+------------+------------+------------+------------+------------+
 *  |                      status                       |                   block_id / flag                 |
 *  ---------------------------------------------------------------------------------------------------------
 *  |EI|reserved | packet_fmt |                               packet_id / reserved                          |
 *  ---------------------------------------------------------------------------------------------------------
 *  |                                           block_id64 (high part)                                      |
 *  ---------------------------------------------------------------------------------------------------------
 *  |                                           block_id64 (low part)                                       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |                                           packet_id32                                                 |
 *  ---------------------------------------------------------------------------------------------------------
 *  |                                           ......                                                      |
 *  +------------+------------+------------+------------+------------+------------+------------+------------+
 */

// typedef struct _GVSP_PACKET_HEADER_
// {
//     unsigned short  status;                     // 16bits��״̬��

//     /*
//      * �ֶ�������EIλ������
//      * 1. ����ʾblock_idʱ��block_id = 0������Ϊ���԰���
//      * 2. ����ʾflagʱ����8bit(0~7) device-specific;
//      * bit (8~12) ��������0;
//      * bit 13, ��1��ʾ GEV_FLAG_RESEND_RANGE_ERROR
//      * bit 14, ��1��ʾ GEV_FLAG_PREVIOUS_BLOCK_DROPPED
//      * bit 15, ��1��ʾ GEV_FLAG_PACKET_RESEND
//      */
//     unsigned short  block_id_or_flag;           // 16bits

//     /*
//      * EI flag = 0 �� block_id����ʾ block_id ʹ��16bits��packet_id ʹ��24bits������չ�ֶ�
//      * EI flag = 1 �� flag����ʾ block_id ʹ��64bits�� packet_id ʹ��32bits��������չ�ֶ�
//      */
//     unsigned char   ei;                         // 1bit
//     unsigned char   reserved;                   // 3bits����0
//     unsigned char   packet_format;              // 4bits, �ο� GVSP�İ����Ͷ���

//     /*
//      * EI flag = 0 �� packet_id����block�е����ݰ�packet_id��ÿ��block��ʼʱ���������packet_id��
//      * EI flag = 1 �� reserved����0
//      */
//     unsigned int    packet_id_or_reserved;      // 24bits

//     // �����ֶν��� EI flag = 1 ʱ��Ч
//     unsigned int    block_id_high;              // 32bits
//     unsigned int    block_id_low;               // 32bits
//     unsigned int    packet_id32;                // 32bits

// } GVSP_PACKET_HEADER;

typedef struct _GVSP_PACKET_HEADER_
{
    unsigned short  status;                     // 16bits��״̬��
    unsigned short  flag;                       // 16bits

    /*
     * EI flag = 0 �� block_id����ʾ block_id ʹ��16bits��packet_id ʹ��24bits������չ�ֶ�
     * EI flag = 1 �� flag����ʾ block_id ʹ��64bits�� packet_id ʹ��32bits��������չ�ֶ�
     */
    unsigned char   ei;                         // 1bit
    unsigned char   packet_format;              // 4bits, �ο� GVSP�İ����Ͷ���

    // �����ֶν��� EI flag = 1 ʱ��Ч
    unsigned int    block_id_high;              // 32bits
    unsigned int    block_id_low;               // 32bits
    unsigned int    packet_id;                  // 32bits

} GVSP_PACKET_HEADER;

#define GVSP_EI0_PACKET_SIZE        8           // GVSP������ͷ����EIλ��0ʱ�ĳ���
#define GVSP_EI1_PACKET_SIZE        20          // GVSP������ͷ����EIλ��1ʱ�ĳ���


// GVSP�İ����ͣ�Ŀǰ����6��ֵ����������
// For Standard Transmission Mode:
#define GVSP_PACKET_FMT_LEADER              0x01
#define GVSP_PACKET_FMT_TRAILER             0x02
#define GVSP_PACKET_FMT_PAYLOAD_GENERIC     0x03
#define GVSP_PACKET_FMT_PAYLOAD_H264        0x05
#define GVSP_PACKET_FMT_PAYLOAD_IMAGE       0x06
// For All-in Transmission Mode:
#define GVSP_PACKET_FMT_ALL_IN              0x04


// GVSP ���ĸ�������
typedef enum _GVSP_PACKET_PAYLOAD_TYPE_
{
    GVSP_PT_UNCOMPRESSED_IMAGE          = 0x0001,
    GVSP_PT_RAW_DATA                    = 0x0002,
    GVSP_PT_FILE                        = 0x0003,
    GVSP_PT_CHUNK_DATA                  = 0x0004,
    GVSP_PT_CHUNK_DATA_EX               = 0x0005,
    GVSP_PT_JPEG                        = 0x0006,   // ITU-T Rec. T.81 �����jpeg��ʽ
    GVSP_PT_JPEG2000                    = 0x0007,   // ITU-T Rec. T.800 �����jpeg��ʽ
    GVSP_PT_H264                        = 0x0008,   // ITU-T Rec. H.264
    GVSP_PT_MULTI_IMAGE                 = 0x0009,

    // bit1 Ϊ1�� ��ʾ ��չ����
    GVSP_PT_UNCOMPRESSED_IMAGE_EX       = 0x4001,
    GVSP_PT_RAW_DATA_EX                 = 0x4002,
    GVSP_PT_FILE_EX                     = 0x4003,
    GVSP_PT_JPEG_EX                     = 0x4006,
    GVSP_PT_JPEG2000_EX                 = 0x4007,
    GVSP_PT_H264_EX                     = 0x4008,
    GVSP_PT_MULTI_IMAGE_EX              = 0x4009,

    // [0x8000, 0xffff]�Զ���

} GVSP_PACKET_PAYLOAD_TYPE;

// additional info for Image Data Leader Packet
typedef struct _GVSP_IMAGE_DATA_LEADER_
{
    struct
    {
        unsigned char   id:4;
        unsigned char   count:4;
    }field;     // ����ʶ�������������ʾ��洢ʱ����Ҫ���� 0-0,1-0;0-1,1-1...��˳��

    unsigned char   reserved;           // ��0
    unsigned short  payload_type;       // 0x0001 or 0x4001
    unsigned int    timestamp_high;
    unsigned int    timestamp_low;
    unsigned int    pixel_format;
    unsigned int    size_x;
    unsigned int    size_y;
    unsigned int    offset_x;
    unsigned int    offset_y;
    unsigned short  padding_x;
    unsigned short  padding_y;
} GVSP_IMAGE_DATA_LEADER;


//////////////////////////////////////GVSP Define /////////////////////////////////////////////

#ifndef GVSP_COMMON

#define GVSP_COMMON         unsigned short status;  \
    union                                           \
    {                                               \
        unsigned short      block_id;               \
        unsigned short      flag;                   \
    };                                              \
    unsigned  char      EIAndPF;                    \
    unsigned  char      packetid[3];                \
    unsigned int        block_id64High;             \
    unsigned int        block_id64Low;              \
    unsigned int        packet_id32;                \

#endif
// GVSP HEAD 2.0��׼
#ifndef TAG_GVSP_HEAD
#define TAG_GVSP_HEAD
typedef struct tagGVSP_Head
{
    GVSP_COMMON

} GVSPHEAD;
#endif



/////////////////////Data Leader define ///////////////////////////////////


// Image Data Leader Define

#ifndef TAG_GVSP_IMAGE_DATA_LEADER
#define TAG_GVSP_IMAGE_DATA_LEADER
typedef struct tagGVSPImageDataLeader
{
    unsigned char   field_id_count;
    unsigned char   reserved;
    unsigned short  payload_type; // 0x0001 or 0x4001
    unsigned int    timestampHigh;
    unsigned int    timestampLow;
    unsigned int    pixel_format;
    unsigned int    size_x;
    unsigned int    size_y;
    unsigned int    offset_x;
    unsigned int    offset_y;
    unsigned short  padding_x;
    unsigned short  padding_y;
} GVSPIMAGEDATALEADER;
#endif

#ifndef TAG_GVSP_IMAGE_DATA_TRAILER
#define TAG_GVSP_IMAGE_DATA_TRAILER
typedef struct tagImageDataTrailer
{
    unsigned short  reserved;
    unsigned short  payload_type; // 0x0001 or 0x4001
    unsigned int    size_y;
} GVSPIMAGEDATATRAILER;
#endif

#endif  // MV_GIGEVISION_MAJOR_VERSION == 2

///////////////////////////////////////////////////////////////////////////




// GVSP packet format
#define MV_DATA_LEADER_FORMAT           0x1
#define MV_DATA_TRAILER_FORMAT          0x2
#define MV_DATA_PAYLOAD_FORMAT_GD       0x3
#define MV_ALL_IN_FORMAT                0x4
#define MV_DATA_PAYLOAD_FORMAT_264      0x5
#define MV_DATA_PAYLOAD_FORMAT_MI       0x6



// payload type
#define MV_PAYLOAD_IMAGE                0x0001
#define MV_PAYLOAD_IMAGE_EX             0x4001
#define MV_PAYLOAD_RAWDATA              0x0002
#define MV_PAYLOAD_RAWDATA_EX           0x4002
#define MV_PAYLOAD_FILE                 0x0003
#define MV_PAYLOAD_FILE_EX              0x4003
#define MV_PAYLOAD_CHUNKDATA            0x0004
#define MV_PAYLOAD_CHUNKDATA_EX         0x0005
#define MV_PAYLOAD_JPEG                 0x0006
#define MV_PAYLOAD_JPEG_EX              0x4006
#define MV_PAYLOAD_JPEG2000             0x0007
#define MV_PAYLOAD_JPEG2000_EX          0x4007
#define MV_PAYLOAD_H264                 0x0008
#define MV_PAYLOAD_H264_EX              0x4008
#define MV_PAYLOAD_MULTIZONEIMAGE       0x0009
#define MV_PAYLOAD_MULTIZONEIMAGE_EX    0x0009
#define MV_PAYLOAD_DEVICESPECIFIC       0x8000  // >= 0x8000���������



#endif
