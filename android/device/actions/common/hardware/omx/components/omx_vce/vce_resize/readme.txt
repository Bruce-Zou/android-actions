������ʵ��yuvsp��ʽ���²������ܣ���Ҫ����4kx2k�Ľ�2�²�����

���˵�����£�

1.  ����ֱ�ӵ���vcedrv��֧�ֶ�instances��֧��gl5202/gl5203/gl5207/gl5202c;

2.  gl5202��gl5203֧�ֽ�1��2��4��8���²�����gl5027(gl5202c��֮��ͬ)֧�ֽ�1��2���²�����

3.  ����bufferΪyuv420sp��ʽ����������8���ض��루stride��16���ض��룩����������16���ض��루����gl5207/gl5202c���������32���ض��룩��

4.  �Ѹ�����ѹ����~android\device\actions\hardware\omx\components\omx_vce\Ŀ¼�� ���롣
    Ĭ��Ϊgl5207((gl5202c)���á�Ҫ����gl5202/gl5203��Ҫ�޸�Android.mk��vce_cfg.h��

5.  ����ͨ�����붯̬���̬�������ã��޸�һ��Android.mk���ɡ�

6.  ��װ�������ӿڣ��Թ����ϲ���ã����Ӽ�main.c���Գ��򣩣�
void* VceReSize_Open(void * input/*VR_Input_t*/);
int   VceReSize_Run(void* handle,void* param/*VR_Parm_t*/);
void  VceReSize_Close(void* handle);
