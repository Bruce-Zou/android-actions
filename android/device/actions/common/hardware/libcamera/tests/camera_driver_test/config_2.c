

#ifndef _CONFIG_1_H
#define _CONFIG_1_H

#include "buf_op.h"
#include "display.h"
#include "device_op.h"


void load_config(void)
{
//camera�豸��Ӧ���豸��
DEV_CAMERA_NAME = "/dev/video0";

//�����ɼ����ݵ�buffer��Ŀ
 BUF_COUNT  = 6;
 //���õ�io��ʽ
 io = IO_METHOD_USERPTR;
 //��camera����dq���ĵڼ�֡������Ϊ��������
 CAPTURE_NUM = 300;
 
// //��ҪԤ����֡��
// TOTAL_PREVIEW_NUM = 300; 
 
/*����ÿ�ֹ���ʱ��ģʽ
*  MODE_ALL������������֧�ֵ�ÿ�ֹ��ܵ����в�ͬģʽ
*  MODE_DEFAULT:�����������ڲ����õ�ÿ�ֹ��ܵ�Ĭ��ģʽ
*/
OPTION_TESTMODE = MODE_ALL_FAST;

/*camera�����֡���ݸ�ʽ
*   V4L2_PIX_FMT_YUV420:
*   V4L2_PIX_FMT_YUV422P:
*   V4L2_PIX_FMT_NV12:
*   V4L2_PIX_FMT_YUYV:
*   V4L2_PIX_FMT_SBGGR8:
*/
FRAME_FORMAT = V4L2_PIX_FMT_YVU420;

//����ȷ������ȡ��֡��С��Ϊframesize_array[]������±�
FRAME_SIZE_ID = 0;


//����֡�ʣ�Ϊframe_interval_array[]������±�
FRAME_RATE_ID = 0;

/*����֡���ݵķ�ʽ
*
*   MODE_DISP���͵�lcd��ʾ
*   MODE_SAVEFILE�������ļ�
*
*/
PROCESS_MODE = MODE_DISP;
//֡���ݱ���·��
DEV_CAMERA_NAME = DEV_CAMERA_NAME;

}







#endif //_CONFIG_1_H
