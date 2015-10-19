

#ifndef _CONFIG_1_H
#define _CONFIG_1_H

#include <linux/videodev2.h>
#include "buf_op.h"
#include "display.h"
#include "device_op.h"
#include "video_dev.h"



void load_config(void)
{
//camera�豸��Ӧ���豸��
DEV_CAMERA_NAME = DEV_CAMERA_BACK_NAME;


DEV_CAMERA_FRONT_NAME = "/dev/video3";
DEV_CAMERA_BACK_NAME = "/dev/video0";


//vout�豸��Ӧ���豸��
DEV_VIDEO_NAME = "/dev/video1";

//�����ɼ����ݵ�buffer��Ŀ
BUF_COUNT  = 5;


 
/**���õ�io��ʽ
*  IO_METHOD_MMAP
*  IO_METHOD_USERPTR
*/
io = IO_METHOD_USERPTR;


 
//��Ҫ��camera��ȡ����֡��
 CAPTURE_NUM = 30;
 
// //��ҪԤ����֡��
// TOTAL_PREVIEW_NUM = 300; 
 
///*����ÿ�ֹ���ʱ��ģʽ
//*  MODE_ALL������������֧�ֵ�ÿ�ֹ��ܵ����в�ͬģʽ
//*  MODE_DEFAULT:�����������ڲ����õ�ÿ�ֹ��ܵ�Ĭ��ģʽ
//*/
//OPTION_TESTMODE = MODE_ALL;




/*camera�����֡���ݸ�ʽ
*   V4L2_PIX_FMT_YUV420:
*   V4L2_PIX_FMT_YUV422P:
*   V4L2_PIX_FMT_NV12: YUV420 semi planar
*   V4L2_PIX_FMT_YUYV:
*   V4L2_PIX_FMT_SBGGR8:
*/
FRAME_FORMAT = V4L2_PIX_FMT_YUV420;



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
PROCESS_MODE = MODE_SAVEFILE;



//֡���ݱ���·��
SAVEFILE_PATH = "/data";



SCREEN_WITDH = 800;
SCREEN_HEIGHT = 480;



}







#endif //_CONFIG_1_H
