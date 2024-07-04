/*
 * UltraSonic.h
 *
 *  Created on: 2023. 3. 22.
 *      Author: user
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_



#define SAMPLE_SIZE 100
//Port register index
#define PC4_BIT_LSB_IDX     3
#define PC5_BIT_LSB_IDX     11
#define P4_BIT_LSB_IDX      4
#define P5_BIT_LSB_IDX      5


// SCU registers
#define LCK_BIT_LSB_IDX     1
#define ENDINIT_BIT_LSB_IDX 0
#define EXIS0_BIT_LSB_IDX   4
#define FEN0_BIT_LSB_IDX    8
#define EIEN0_BIT_LSB_IDX   11
#define IGP0_BIT_LSB_IDX    14
#define REN0_BIT_LSB_IDX    9


// CCU60 registers
#define DISS_BIT_LSB_IDX    1
#define DISR_BIT_LSB_IDX    0
#define CTM_BIT_LSB_IDX     7
#define T12PRE_BIT_LSB_IDX  3
#define T12CLK_BIT_LSB_IDX  0
#define T12STR_BIT_LSB_IDX  6
#define T12RS_BIT_LSB_IDX   1
#define INPT12_BIT_LSB_IDX  10
#define ENT12PM_BIT_LSB_IDX 7


// SRC registers
#define SRPN_BIT_LSB_IDX    0
#define TOS_BIT_LSB_IDX     11
#define SRE_BIT_LSB_IDX     10
#define INP0_BIT_LSB_IDX    12


void initCCU60(void);
void initCCU61(void);
void initERU(void);
void initUSonic(void);
void usonicTrigger(void);
void ERU0_ISR(void);
void CCU60_T12_ISR(void);

extern float new_g_range;
extern float old_g_range;

extern unsigned int g_range;
extern unsigned char g_range_valid_flag;

#endif /* ULTRASONIC_H_ */
