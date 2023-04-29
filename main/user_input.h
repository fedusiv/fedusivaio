#ifndef __USER_INPUT_H__
#define __USER_INPUT_H__

#define JOYSTICK_POS_LEFT 0x01
#define JOYSTICK_POS_RIGHT 0x02
#define JOYSTICK_POS_UP 0x04
#define JOYSTICK_POS_DOWN 0x08

void xUserInputTask(void * task_parameter);


#endif // __USER_INPUT_H__
