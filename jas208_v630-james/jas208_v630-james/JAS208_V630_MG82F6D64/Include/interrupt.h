#ifndef _INTERRUPT_H
#define _INTERRUPT_H

//============================================================================//
// Include
//============================================================================//

//============================================================================//
// Function
//============================================================================//
void SF_IntHandler(void);
void INT0_IntHandler(void);
void INT1_IntHandler(void);
void INT2_IntHandler(void);
void T0_IntHandler(void);
void TWI0_IntHandler(void);
void UART0_IntHandler(void);
void UART1_IntHandler(void);
void UART2_IntHandler(void);
void UART3_IntHandler(void);


#endif // INTERRUPT_H