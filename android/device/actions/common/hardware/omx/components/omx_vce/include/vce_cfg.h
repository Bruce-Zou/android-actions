#ifndef __VCE_CFG_H__
#define __VCE_CFG_H__

/*�ֶ�����IC����:ѡȡһ��*/
//#define  IC_TYPE_GL5202
//#define  IC_TYPE_GL5203
//#define  IC_TYPE_GL5207
//#define  IC_TYPE_GL5206
#define  IC_TYPE_GL5209

/*����IC����,�Զ��趨��*/
#define  FIXED_CTS_4_4
#define  MOD_FOR_UNALIGN_RES

#ifdef   IC_TYPE_GL5209
#define IC_TYPE_GL5206
#endif
#ifdef   IC_TYPE_GL5206
#define  IC_TYPE_GL5207
#endif
#ifdef   IC_TYPE_GL5207
#define  IC_TYPE_GL5203
#endif
#ifdef   IC_TYPE_GL5203
#define  enable_gralloc
#endif

#endif
