

/****
*         ���������������������̵Ĳ��ԡ�
*��Ҫ���Ե㣺
*  1.����ģʽ�¸����ܵ��趨�Ƿ�������
*    �����ܰ������ع�ģʽ����ƽ�⡢���ȡ�����Ч��
*  2.���պ�����������Ƭ���鿴�Ƿ�������
*  3.�������յĺ�ʱ������Ƿ��ں���Χ��
*
*
*/
#include "interface.h"


int main()
{
  load_config();
  
  
  
//  camera_back_in_use();
//  camera_open();
//  camera_get_ctrls();
//  camera_set_ctrls_all_fast();
//  camera_set_ctrls_default_fast();
//  camera_get_framesize();
//  camera_set_format();
//  //camera_get_framerate();
// // camera_set_frame_rate();
//  camera_start();
//
////     camera_set_ctrls_all_slow();
////  camera_set_ctrls_default_slow();
//     camera_stop();
//     camera_close();
//  report_capture_time();
  
  
  
  camera_front_in_use();
  camera_open();
  camera_get_ctrls();
  camera_set_ctrls_all_fast();
  camera_set_ctrls_default_fast();
  camera_get_framesize();
  camera_set_format();
  camera_get_framerate();
     camera_set_frame_rate();
  camera_start();

//     camera_set_ctrls_all_slow();
//  camera_set_ctrls_default_slow();
     camera_stop();
     camera_close();
  report_capture_time();
  return 0;
  
  
}
