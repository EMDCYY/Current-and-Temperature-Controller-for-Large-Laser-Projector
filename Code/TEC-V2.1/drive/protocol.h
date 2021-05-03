#ifndef __Protocol_H
#define	__Portocol_H

#include "stm32f0xx.h"
#include "stdbool.h"
void protocol(void);
bool ShutDown_Condition(void);
bool StartUp_Condition(void);
bool Tune_Condition(void);
bool TEC_Condition(void);
#endif /* __Portocol_H */
