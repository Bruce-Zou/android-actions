

/****
*         ��������������Ԥ�����̵Ĳ��ԡ�
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
  camera_open();
  camera_get_ctrls();
  camera_set_params();
  camera_get_framesize();
  camera_set_format();
  camera_start();
  camera_stop();
  camera_close();
  report_frame_rate();
  return 0;
}
