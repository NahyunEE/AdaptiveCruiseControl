/*
 * UltraSonic.c
 *
 *  Created on: 2023. 3. 22.
 *      Author: user
 */

//trigger = d7, echo = d33;
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "IfxCcu6_reg.h"
#include "IfxVadc_reg.h"
#include "IfxGtm_reg.h"
#include "Ultrasonic.h"
#include "IfxScuEru.h"
#include <Libraries\iLLD\TC27D\Tricore\Cpu\Irq\IfxCpu_Irq.h>
#include "IfxPort.h"

unsigned int g_range;
unsigned char g_range_valid_flag;

float new_g_range=0;
float old_g_range=0;

__interrupt(0x0A) __vector_table(0)

//Echo 신호가 상승 엣지나 하강 엣지를 감지할 때 호출
void ERU0_ISR(void)
{
    if ((P00_IN.U &(0x1 << P4_BIT_LSB_IDX)) != 0)//ECHO 상승 엣지 감지
    {
        //start CCU61 T12 counter
        //상승edge 감지되면 CCU61 모듈의 T12 타이머 시작
        CCU61_TCTR4.U = 0x1 << T12RS_BIT_LSB_IDX;
    }
    else //ECHO 하강 엣지 감지
    {
        CCU61_TCTR4.B.T12RR = 0x1; //ECHO 하강엣지 감지되면 CCU61의 T12 타이머 중지
        new_g_range = ((CCU61_T12.B.T12CV * 1000000)/48828) /(58*2);

        //(타이머의 현재 값은 Echo 신호의 시작부터 끝까지의 지속 시간)
        // 타이머 클럭 속도 48828, 58 =  측정단위 변환(cm단위)
        g_range=(unsigned int)old_g_range;

        g_range_valid_flag = 1; //거리 측정이 유효하여 플래그
        CCU61_TCTR4.B.T12RES = 0x1; // 타이머 초기화
    }
    old_g_range = new_g_range;
}

// CCU60_T12_ISR 함수는 CCU60 타이머 12(T12)의 인터럽트 서비스 루틴
__interrupt(0x0B) __vector_table(0)
void CCU60_T12_ISR(void)
{
    // P02 핀의 출력을 클리어합니다.
    P02_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX);
}

void initERU(void) //echo핀 인터럽트 핸들러
{
    //SCU(EICR1, IGCR0) 및 SRC(SCU_SCU_ERU0)를 사용하여 인터럽트 설정과 우선순위를 지정
    SCU_EICR1.U &= ~(0x7 << EXIS0_BIT_LSB_IDX);
    SCU_EICR1.U |= 0x02 << EXIS0_BIT_LSB_IDX;

    SCU_EICR1.U |= 0x01 << FEN0_BIT_LSB_IDX; //falling edge
    SCU_EICR1.U |= 0x01 << REN0_BIT_LSB_IDX; //rising edge

    SCU_EICR1.U |= 0x01 << EIEN0_BIT_LSB_IDX;

    SCU_EICR1.U &= ~(0x7 << INP0_BIT_LSB_IDX);

    SCU_IGCR0.U &= ~(0x03 << IGP0_BIT_LSB_IDX);
    SCU_IGCR0.U |= 0x01 << IGP0_BIT_LSB_IDX;

    // SRC interrupt setting
    SRC_SCU_SCU_ERU0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_SCU_SCU_ERU0.U |= (0x0A << SRPN_BIT_LSB_IDX);

    SRC_SCU_SCU_ERU0.U |= 0x01 << SRE_BIT_LSB_IDX;

    SRC_SCU_SCU_ERU0.U &= ~(0x03 << TOS_BIT_LSB_IDX);
}

// CCU60 타이머 초기화 함수
void initCCU60(void) {
    // SCU_WDTCPU0_CON0의 암호 액세스를 언락
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);   // 언락될 때까지 대기

    // ENDINIT를 지우기 위한 수정 액세스
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);  // 잠길 때까지 대기

    CCU60_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);    // CCU60 활성화

    // SCU_WDTCPU0_CON0의 암호 액세스를 언락
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);   // 언락될 때까지 대기

    // ENDINIT를 지우기 위한 수정 액세스
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);  // 잠길 때까지 대기

    // CCU60 T12 설정
    while((CCU60_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);    // CCU60 모듈 대기

    CCU60_TCTR0.U &= ~(0x7 << T12CLK_BIT_LSB_IDX);  // f_T12 = f_CCU6 / 프리스케일러
    CCU60_TCTR0.U |= 0x2 << T12CLK_BIT_LSB_IDX;     // f_CCU6 = 50MHz, 프리스케일러 = 4
    // CCU60_TCTR0.U |= 0x1 << T12PRE_BIT_LSB_IDX;     // f_T12 = 48,828 Hz

    CCU60_TCTR0.U &= ~(0x1 << CTM_BIT_LSB_IDX);     // PM 발생 시 T12 자동 리셋

    CCU60_T12PR.U = 125 -1;                       // PM 인터럽트 주파수 = f_T12 / (T12PR + 1)
    CCU60_TCTR4.U |= 0x1 << T12STR_BIT_LSB_IDX;     // T12PR을 셰도우 레지스터에서 로드

    CCU60_TCTR2.B.T12SSC = 0x1; // Single Shot Control

    CCU60_T12.U = 0;    // T12 카운터 레지스터를 지움

    // CCU60 T12 PM 인터럽트 설정
    CCU60_IEN.U |= 0x1 << ENT12PM_BIT_LSB_IDX;      // T12 PM 인터럽트 활성화
    CCU60_INP.U &= ~(0x3 << INPT12_BIT_LSB_IDX);    // 서비스 요청 출력 SR0 선택

    // CCU60을 위한 SRC 설정
    SRC_CCU6_CCU60_SR0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_CCU6_CCU60_SR0.U |= 0x0B << SRPN_BIT_LSB_IDX;       // 우선순위 0x0B로 설정
    SRC_CCU6_CCU60_SR0.U &= ~(0x3 << TOS_BIT_LSB_IDX);      // CPU0이 T12 PM 인터럽트 서비스
    SRC_CCU6_CCU60_SR0.U |= 0x1 << SRE_BIT_LSB_IDX;         // SR0 활성화
}

// CCU61 타이머 초기화 함수
void initCCU61(void) {
    // SCU_WDTCPU0_CON0의 암호 액세스를 언락합니다.
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);   // 언락될 때까지 대기

    // ENDINIT를 지우기 위한 수정 액세스
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);  // 잠길 때까지 대기

    CCU61_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);    // CCU61 활성화

    // SCU_WDTCPU0_CON0의 암호 액세스를 언락합니다.
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);   // 언락될 때까지 대기

    // ENDINIT를 지우기 위한 수정 액세스
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);  // 잠길 때까지 대기

    // CCU60 T12 설정
    while((CCU61_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);    // CCU60 모듈 대기

    CCU61_TCTR0.U &= ~(0x7 << T12CLK_BIT_LSB_IDX);  // f_T12 = f_CCU6 / 프리스케일러
    CCU61_TCTR0.U |= 0x2 << T12CLK_BIT_LSB_IDX;     // f_CCU6 = 50MHz, 프리스케일러 = 1024
    CCU61_TCTR0.U |= 0x1 << T12PRE_BIT_LSB_IDX;     // f_T12 = 48,828 Hz

    CCU61_T12PR.U = 100000 -1;                       // PM 인터럽트 주파수 = f_T12 / (T12PR + 1)
    CCU61_TCTR4.U |= 0x1 << T12STR_BIT_LSB_IDX;     // T12PR을 셰도우 레지스터에서 로드

    CCU61_T12.U = 0;    // T12 카운터 레지스터 지움
}

void initUSonic(void)
{
    // P02.5 핀을 출력으로 설정하고 초기 상태를 Low로 설정
    IfxPort_setPinMode(&MODULE_P02, 5, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(&MODULE_P02, 5, IfxPort_State_low);

    // P00.4 핀을 입력으로 설정
    IfxPort_setPinMode(&MODULE_P00, 4, IfxPort_Mode_inputPullDown);
}

// Trigger 핀(P02.5)에 10µs의 고주파 펄스를 발생시켜 초음파 신호가 전송
void usonicTrigger(void)
{
    //start of 10us Trigger Pulse
    //GPIO_P02.5 -> HIGH
    P02_OUT.U |= 0x1 << P5_BIT_LSB_IDX;
    g_range_valid_flag = 0;

    CCU60_TCTR4.U = 0x1 << T12RS_BIT_LSB_IDX;       // T12 start counting
}

