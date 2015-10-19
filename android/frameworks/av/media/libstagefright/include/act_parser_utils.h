/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ACT_PARSER_UTILS_H_

#define ACT_PARSER_UTILS_H_

#include <utils/Errors.h>

namespace android {

typedef struct
{	 
	int32_t format; 			/* pcm������ */
	int32_t channels;			/* ������ */
	int32_t samples_per_frame;	/* ÿ֡������������ */
	int32_t bits_per_sample;	/* ÿ���������������Ӧ������bit�� */
	int32_t bytes_per_frame;	/* ÿ֡������bytes�� */
} parser_pcm_t;

typedef struct{
	/*������*/
	uint32_t header_type;
	/*���غɳ���*/
	uint32_t block_len;
	/*�����ļ�λ��*/
	uint32_t packet_offset;
	/*����ʱ���*/
	uint32_t packet_ts;
	/*�����ֶ�1*/
	uint32_t reserved1;
	/*�����ֶ�2*/
	uint32_t reserved2;
	/*h264�õ���ȷʱ�����Ҫ����Ϣ*/
	uint8_t stream_end_flag;/*1Ϊ������ 0Ϊ��*/
	uint8_t parser_format;
	uint8_t seek_reset_flag;
	uint8_t reserved_byte2;
}packet_header_t;

typedef struct tVideoParam{
	/*	Video properties	*/	
	uint32_t	uiFOURCCCompressed;//FOURCC_WMV3	
	uint32_t	fltFrameRate;//0 
	uint32_t	fltBitRate;//0    
	uint32_t 	iWidthSource;
	uint32_t	iHeightSource;
	uint32_t    iPostFilterLevel;			//-1	
	uint8_t	*	pSequenceHeader;
	uint32_t	uipSequenceHeaderLength;
	uint32_t	bHostDeinterlace ;			// 1
}VIDEOPARAM;

typedef struct tVc1VideoParam{
 	VIDEOPARAM wmv_param;
 	uint8_t video_param[28 + 128];
}VC1VIDEOPARAM;

typedef struct
{
	parser_pcm_t	pcm_param;
	packet_header_t packet;
}pack_pcm_init_t;
	
typedef struct
{
	int32_t FlavorIndex;
	int32_t BitsPerFrame;
	int32_t Channels;	
	int32_t version;
	int32_t samples;
	int32_t regions;
	int32_t delay;
	int32_t cpl_start;
	int32_t cpl_qbits;
} parser_ra_t;

typedef struct
{
    uint16_t FlavorIndex;
    uint32_t InterleaverID;         /* interleaver 4cc */
    uint32_t CodecID;               /* codec 4cc */
    uint32_t Granularity;           /* size of one block of encoded data */
    uint32_t InterleaveFactor;      /* blocks per superblock */
    uint32_t InterleaveBlockSize;   /* size of one block */
    uint32_t FramesPerBlock;        /* frames per block */
    uint32_t NumCodecFrames;        /* frames per superblock */
    uint32_t CodecFrameSize;        /* size of one frame (bytes) */
    uint32_t CodecFrameBits;        /* size of one frame (bits), in sipro, not integer byte in one frame */
    uint32_t OpaqueDataSize;
    uint32_t PatternAvail;          /* interleave pattern avail flag, 0:no, 1:already set */
    int8_t   *OpaqueData;
    uint16_t *InterleavePattern;

    uint32_t last_timestamp;        /* �ϴ�chunk��ʱ��� */
    uint32_t total_time;            /* milli seconds */
    uint32_t avg_bitrate;           /* bps */
    uint32_t sample_rate;    
    uint32_t channel;
    uint32_t frames_per_chunk;
    uint32_t chunk_size;
    uint32_t chunk_time;
    uint32_t total_chunk;           /* total chunk, duration of a chunk is nearly 1-2s */
    uint32_t *chunksize_tab;        /* ��10λ��ʾ��ǰchunkʱ�䣬��20λ��ʾ��ǰchunk�Ĵ�С */
    uint32_t chunk_idx;

    uint32_t datafilepos;  //cz_20090908 ���Ӽ�¼rm������ʼλ�ã�Ϊ�˿����ʹ��  
    uint32_t lastframe_flag;  //cz_20090908��ʶ���һ֡�������������û���������
} radec_parser_info_t;

typedef struct{    
	uint32_t min_blocksize;
	uint32_t max_blocksize;
	uint32_t min_framesize;
	uint32_t max_framesize;
	uint32_t sample_rate;
	uint32_t channels;
	uint32_t bits_per_sample;
	uint64_t total_samples;
	uint8_t md5sum[16];
} parser_flac_t;

/* repack frames */
typedef struct {
	uint32_t ulFrameSize;
	uint32_t ulTimeStamp;
	uint16_t ulSequenceNum;
	uint16_t usFlags;
	uint32_t bLastPacket;
	uint32_t ulNumSegments;
} fh_t;

typedef struct {
uint32_t ulLength; 
uint32_t ulMOFTag; 
uint32_t ulSubMOFTag;
uint16_t usWidth;
uint16_t usHeight;
uint16_t usBitCount;
uint16_t usPadWidth;
uint16_t usPadHeight;
uint8_t ubfr[4]; /* �����п��ֽ� */
} rv_header_t;

typedef struct tagBITMAPINFOHEADER {
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
}BITMAPINFOHEADER;

typedef struct {
	uint16_t  wFormatTag;
	uint16_t  nChannels;
	uint32_t  nSamplesPerSec;
	uint32_t  nAvgBytesPerSec;
	uint16_t  nBlockAlign;
	uint16_t  wBitsPerSample;
	uint16_t  cbSize;
	union {        
		uint16_t wValidBitsPerSample;
		uint16_t wSamplesPerBlock;
		uint16_t wReserved;    
		} Samples;    
	uint32_t        dwChannelMask;
	uint8_t         SubFormat[12]; /* GUID type, may not be 8 bytes long -mmg */
}WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;

typedef struct tAudioParam{
	/*	Audio properties	*/
	unsigned int	nVersion;
	unsigned int	nSamplesPerSec;
	unsigned int   	nAvgBytesPerSec;
	unsigned int    nBlockAlign;
	unsigned int    nChannels;
	unsigned int  	nSamplesPerBlock;
	unsigned int   	nEncodeOpt;
	/*	ASF Header	*/    
	unsigned int    dwChannelMask;
	unsigned int	cbHeader;
	unsigned int  	cbPacketSize;
	unsigned int   	cPackets;
	unsigned int	nBitsPerSample;
	unsigned int  	wAudioStreamId;
	unsigned int	TotalTime;
	unsigned int	cbPreroll;

	/* ASF Internal Format*/
        unsigned int    wFormatTag;//0x160-0x163,etc

}AUDIOPARAM;

}  // namespace android

#endif  // AVC_UTILS_H_
