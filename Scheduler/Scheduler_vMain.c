/***************************************************************************
*=====================      By Don Issac      ===============
****************************************************************************
* Title        : Scheduler non-preemptive
*
* Description  :
*
* Proyect      :
*
*
* Environment  : MKL43Z256xxx4 board
*
* Responsible  : Issac Jiménez
*
*****************************************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>

#include "Sys_Init.h"
#include "Headerss.h"


#define PIT_LED_HANDLER PIT_IRQHandler
#define SCH_nTASKS 3 /*number of tasks*/


typedef struct
{
	void(*TaskFunction) (void);
	uint16_t u16CurrentTaskTime;
	uint16_t u16DeadLineTaskTime;
	uint16_t u16PeriodTaskTime;
	SHC_nStateTask  nFlagState;
}SCH_tstTaskStruct;

volatile bool SCH_bopitIsrFlag = false;
static SCH_tstTaskStruct _astTasks[SCH_nTASKS] = SHC_anTASK_DEFINITION; /* Set array of struct to task definitions*/


void PIT_LED_HANDLER(void) /*Invoked every 1ms*/
{
	uint16_t u16TaskIndex = 0;

    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag); /* Clear interrupt flag.*/
    SCH_bopitIsrFlag = true;

    while(SCH_nTASKS != u16TaskIndex)
   	{
    	if(_astTasks[u16TaskIndex].u16CurrentTaskTime == (_astTasks[u16TaskIndex].u16DeadLineTaskTime - 1))
    	{
    		if((enRunning == _astTasks[u16TaskIndex].nFlagState) || (enNotFinished == _astTasks[u16TaskIndex].nFlagState))
			{
    			_astTasks[u16TaskIndex].nFlagState = enNotFinished; /*Task has not been finished*/
			}
    	}

   		if( _astTasks[u16TaskIndex].u16CurrentTaskTime == (_astTasks[u16TaskIndex].u16PeriodTaskTime - 1) )
   		{
   			_astTasks[u16TaskIndex].nFlagState = enReady;
   			_astTasks[u16TaskIndex].u16CurrentTaskTime = 0; /*Reset current time*/
   		}
   		else
   		{
   			_astTasks[u16TaskIndex].u16CurrentTaskTime++;
   		}

   		u16TaskIndex++;
   	}
}


int main(void)
{
	uint16_t u16TaskIndex = 0;

	/*Function created to initialize pines of I/O and PIT(Periodic interrupt timer)*/
	SI_vInitSystem(); /*Initialize I/O and PIT(Periodic Interrupt Timer)*/

	while(1)	/* Enter an infinite loop. */
	{
		if(true == SCH_bopitIsrFlag)
		{
			while(u16TaskIndex != SCH_nTASKS)
			{
				if(enReady ==  _astTasks[u16TaskIndex].nFlagState)
				{
						_astTasks[u16TaskIndex].nFlagState = enRunning;
						_astTasks[u16TaskIndex].TaskFunction(); /* Call task*/
						_astTasks[u16TaskIndex].nFlagState = enSuspended;
				}

			u16TaskIndex++;
			}

			u16TaskIndex = 0;
			SCH_bopitIsrFlag = false;
		}

		else{/*Interrupt has not been invoked */
		}
	}

	return 0;
}
