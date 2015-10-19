/*
 *         camera device operate head file
 *  mainly including device open close.
 *
 *
 *
 */
 
#ifndef _DEVICE_OP_H
#define _DEVICE_OP_H

#include <linux/videodev2.h>

//extern int fd_camera;

typedef struct camera_info{
struct v4l2_format f;
}camera_info_t;

typedef enum{

/*����Ĭ�����ԣ�������ɺ��ͣ��һ��ʱ��*/
MODE_DEFAULT_SLOW = 0,
/*��������֧�ַ�Χ�ڵ�������Чֵ��������ɺ��ͣ��һ��ʱ��*/
MODE_ALL_SLOW,
/*����Ĭ�����ԣ����óɹ���ͣ����ֱ�ӽ���һ�¹�������*/
MODE_DEFAULT_FAST,
/*��������֧�ַ�Χ�ڵ�������Чֵ�����óɹ���ͣ����ֱ�ӽ���һ�¹�������*/
MODE_ALL_FAST,
}option_mode;

extern struct v4l2_queryctrl query_array[];
extern struct v4l2_frmsizeenum framesize_array[];
extern struct v4l2_frmivalenum frame_interval_array[];
extern  int query_array_size;
extern char *DEV_CAMERA_NAME;
extern char *DEV_CAMERA_FRONT_NAME;
extern char *DEV_CAMERA_BACK_NAME;
extern int fd_camera;
extern int FRAME_FORMAT;
extern int FRAME_SIZE_ID;
extern int FRAME_RATE_ID;
extern option_mode OPTION_TESTMODE;


extern int open_device_camera(void);
extern int close_device_camera(void);
extern int queryctrl_camera(struct v4l2_queryctrl* query);
extern int setctrl_camera(struct v4l2_queryctrl *query_value, option_mode mode);
extern int stream_on_camera(void);
extern int stream_off_camera(void);
extern int enum_framesize_camera(struct v4l2_frmsizeenum* framesize);
extern int enum_frameinterval_camera(struct v4l2_frmivalenum* frame_interval);
extern int set_format_camera(struct v4l2_format *format);
extern int set_frame_rate_camera(struct v4l2_streamparm *parms);
extern void errno_exit(const char *s);

extern void init_framesize_array(void);

extern void init_frame_interval_array(void);





#endif  //_DEVICE_OP_H
