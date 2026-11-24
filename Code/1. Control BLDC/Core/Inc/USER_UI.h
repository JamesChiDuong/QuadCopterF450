/*
 * USER_UI.h
 *
 *  Status screen on the on-board LCD of the STM32F429I-DISC1.
 *  Runs in its own low priority task, reads Sys_GetStatus() only.
 */
/*
 * OVERVIEW
 *  - One public function: the UI task body. main.c creates the task
 *    (priority BelowNormal, stack UI_TASK_STACK_WORDS) when UI_LCD_ENABLE = 1.
 *  - The task refreshes the LCD at 10 Hz (state, attitude + artificial
 *    horizon, motor outputs, RC / UART / gamepad input, last event, loop dt).
 *  - It never writes to flight data, so it cannot affect flight control.
 *  - Layout and drawing tricks: see USER_UI.c.
 */

#ifndef INC_USER_UI_H_
#define INC_USER_UI_H_

/* Task body (never returns). Started from main.c when UI_LCD_ENABLE = 1. */
void UI_Task(void const *argument);

#endif /* INC_USER_UI_H_ */
