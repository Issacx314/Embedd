/******************************************************************
 * Headerss.h
 *
 * Author: Issac
 *****************************************************************/

#ifndef HEADERSS_H_
#define HEADERSS_H_

#include "Sys_Init.h"

#define SHC_anTASK_DEFINITION { {LED_vRed,			\
                                11,					\
								2,					\
                                12,					\
							    0					\
                               },					\
                               {LED_vGreen,			\
								23,					\
								4,					\
								24,					\
								0					\
                               },					\
							   {TIMEn_vMain,		\
								47,					\
								6,					\
								48,					\
								0					\
							    }}

typedef enum
{
	enSuspended,
	enReady,
	enRunning,
	enNotFinished
}SHC_nStateTask;

void LED_vRed();

void LED_vGreen();

void TIMEn_vMain();



#endif /* HEADERSS_H_ */