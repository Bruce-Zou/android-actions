/********************************************************************************
 *                              usdk1100
 *                            Module: Face application
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       noahkong  2010-03-11 14:47     1.0             build this file
 ********************************************************************************/
/*!
 * \file     faceApp.h
 * \brief    �������м���Ľӿڼ�����
 * \author   ���ĺ�
 * \version 1.0
 * \date  2010/03/11
 *******************************************************************************/
#ifndef __ALFACE_ON_H__
#define	__ALFACE_ON_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      ��������
 */
typedef struct
{
	/*! ���ϽǺ������� */
	int x;
	/*! ���Ͻ��������� */
	int y;
	/* ���ο� */
	int w;
	/* ���θ� */
	int h;
} DRect;

/*!
 * \brief
 *      ��
 */
typedef struct
{
	/*! �������� */
	int x;
	/*! �������� */
	int y;
} DPoint;

/*!
 * \brief
 *      ������Ч����
 *      Ĭ�ϱ�ʾ������ӵȵ��������꣬ͼ������Ϊ������ģ����
 */
typedef enum
{
	/*! ����Ч */
	MMM_DISABLE_ALL,
	/*! ��ͷ�� */
	MMM_PHOTO_FRAME = 0x1,
	/*! ������Ч */
	MMM_SEYE_MASK = 0x101,
	/*! ˫����Ч�����۾��� */
	MMM_DEYE_MASK,
	/*! �����Ч�����ۺ�������λ�ñ궨 */
	MMM_NOSE_MASK,
	/*! �����Ч����������ı궨 */
	MMM_MOUTHUP_MASK,
	/*! �º�����Ч�����º����ϱ�Ե���ı궨 */
	MMM_MOUTH_MASK,
	/*! �Ϻ�����Ч ���ú������ı궨���ݲ�֧�֣�*/
	MMM_FACE_MASK,
	/*! ������Ч���ñ����ı궨 ���ݲ�֧�֣�*/
	MMM_MOUTHDOWN_MASK,
	/*! ��ͷ��Ч�����ݲ�֧�֣� */
	MMM_FOREHEAD_MASK,
	/*! ͷ����Ч�� ���ݲ�֧�֣�*/
	MMM_HEAD_MASK,
	/*! ���պ��� */
	MMM_BREAD_MASK,
} ALMasque_type_t;

/*!
 * \brief
 *      ��ģ��Ч������ͬʱ��Ҫ��ΪJPG encoder��ʼ������
 */
typedef struct
{
	ALMasque_type_t mas_type;
	int mas_enable;
	short width;
	short height;
	unsigned char *im_yv12;
	unsigned char *im_alpha;
	/*! ˫�����ģ������� */
	DPoint CPi[3];
} ALMask_info_t;

/*!
 * \brief
 *      ������ЧӦ�ó�ʼ���ṹ
 */
typedef struct
{
	/*! ͼ��� */
	int width;
	/*! ͼ��� */
	int height;
	/*! ���ݲ��� */
	int bk_param;
} ALFace_app_init_t;

/*!
 * \brief
 *      ����ʹ�������⺬����ͼ��
 */
typedef struct
{
	/*! ��һ֡ͼ�� */
	unsigned char *pre_img;
	/*! ��ǰͼ�� */
	unsigned char *cur_img;
	/*! ͼ��� */
	int width;
	/*! ͼ��� */
	int height;
	/*! ���������Ŀ */
	int max_faces;
	/**/
	int photoframe_enable;
	/*! ����ͼ��� */
	int bk_width;
	/*! ����ͼ��� */
	int bk_height;
	/*! ����ͼ��buffer */
	unsigned char *bk_img;
	/*! image format YUV420p or yuv420ps */
	int img_fmt;
	int stride;
#if 1
	int dir;         /*pad������ת�Ƕ�*/
	int isfront;  /*�Ƿ���ǰ������ͷ*/
#endif
} ALFace_img_t;

/*!
 * \brief
 *      ����������
 */
typedef struct
{
	/*! �������� */
	DRect *rt;
	/*! ��⵽���������� */
	int RectNum;
	/*������ָ�� */
	unsigned int ptr[2];
} ALDface_out_t;

/*!
 * \brief
 *      ���ۼ�����
 */
typedef struct
{
	/*! ���ۺ�ѡ���� */
	DPoint *lrt;
	/*! ���ۺ�ѡ���� */
	DPoint *rrt;
	/*! ���۸��� */
	int LRectNum;
	/*! ���۸��� */
	int RRectNum;
	/*! ����ָ�� */
	unsigned int ptr[2];
} ALDeye_out_t;

/*!
 * \brief
 *      ���������
 */
typedef struct
{
	/*! �ϴ� */
	DPoint tP;
	/*! �´� */
	DPoint bP;
	/*! ����� */
	DPoint lP;
	/*! ����� */
	DPoint rP;
	/*! ���� */
	DPoint cP;
	/*! ����ָ�� */
	unsigned int ptr[2];
} ALDmouth_out_t;

/*!
 * \brief
 *      ��⵽�����ο���Ϣ
 */
typedef struct
{
	/* ����ͼ��buf */
	unsigned char *buf;
	/* ���ͼ��� */
	unsigned int width;
	/* ���ͼ��� */
	unsigned int height;
	/*! Ц��״̬ */
	int smile_status;
	/*! ������������ 0...;-1Ϊ�������� */
	int faceidx;
	/*! �����������Ϣ */
	ALDface_out_t faceout;
	/*! �����������Ϣ */
	ALDeye_out_t eyeout;
	/*! �����������Ϣ */
	ALDmouth_out_t *mouthout;
	/*! ��Ч��ģ���� */
	int mask_num;
	/*! ��ģ��Ϣ */
	ALMask_info_t *face_mask;
} ALFace_appout_t;

typedef struct
{
	ALFace_img_t *fim;
	ALFace_appout_t fout;
} ALFace_mask_param_t;

typedef enum
{
	/*! ʹ�ܴ�ͷ�� */
	FSET_PHOTO_MASK_ENABLE = 0x1,
	FSET_PHOTO_MASK_DISABLE = 0x2,
	FSET_FACE_GEN = 0x100,
	/*! ʹ��������� */
	FSET_FACE_DET_A = 0x101,
	/*! HAAR��� */
	FSET_FACE_DET_B,
	FSET_FACE_DISABLE,
	/*! ʹ��Ц����� */
	FSET_SIMLE_DET,
	FSET_SIMLE_DISABLE,
	/*! �ر�������Ч */
	FSET_MASK_DISABLE,
	FSET_MASK_ENABLE,
	FGET_STATUS,
	FSET_RESET,
	/*! �۾���Ч */
	//	FSET_EYE_MASK,
	//	/*! �����Ч */
	//	FSET_FACE_MASK,
	//	/*! ������Ч */
	//	FSET_NOSE_MASK,
	//	/*! �����Ч */
	//	FSET_MOUTH_MASK,
	//	/*! ������Ч */
	//	FSET_MOUTHUP_MASK,
	//	/*! ������Ч */
	//	FSET_MOUTHDOWN_MASK,
	//	/*! ��ͷ��Ч */
	//	FSET_FOREHEAD_MASK,
	//	/*! ͷ����Ч */
	//	FSET_HEAD_MASK,
	/*! �������� */
	FSET_DRAW_FACE,
	FSET_FACE_GRAY,
} ALFace_cmd_t;

#if 0
/*! ����Ӧ�ó�ʼ�� */
void *face_app_init(ALFace_app_init_t *);
/*! ����Ӧ������ */
int face_app_run(void *handle,unsigned int cmd,unsigned long args);
/*! ����Ӧ���ͷ���Դ */
int face_app_dispose(void *handle);
int face_mask_run(void *handle,unsigned int cmd,unsigned long args);
#endif
#endif
