#ifndef _USER_MAIN_H_
#define _USER_MAIN_H_




typedef struct MOTOR Motor; // 前向声明



void user_main(void);
void FOC_Process(Motor* motor);
void Print_Process(void);

#endif /* _USER_MAIN_H_ */


