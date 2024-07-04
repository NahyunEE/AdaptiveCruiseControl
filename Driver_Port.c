/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Driver_Port.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/


/***********************************************************************/
/*Variable*/ 
/***********************************************************************/


/***********************************************************************/
/*Function*/ 
/***********************************************************************/
void Driver_Port_Init(void)
{
    // IfxPort_P10_2 핀을 출력 모드로 설정 및 push-pull 출력 모드를 사용하여 일반 출력으로 설정
    IfxPort_setPinModeOutput(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    // IfxPort_P10_2 핀을 High Level 설정하여 출력 활성화
    IfxPort_setPinHigh(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);

    // IfxPort_P10_1 핀을 출력 모드로 설정 및 push-pull 출력 모드를 사용하여 일반 출력으로 설정
    IfxPort_setPinModeOutput(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    // IfxPort_P10_1 핀을 Low Level 설정하여 출력 비활성화
    IfxPort_setPinLow(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex);


    //*오른쪽 바퀴와 왼쪽 바퀴의 SensorA, sensorB를 input을 설정

    //Channel A Sensor A
    //Right
    //Pin23
    IfxPort_setPinModeInput(IfxPort_P14_1.port, IfxPort_P14_1.pinIndex, IfxPort_InputMode_pullDown );

    //Channel A Sensor B
    //Right
    //Pin22
    IfxPort_setPinModeInput(IfxPort_P14_0.port, IfxPort_P14_0.pinIndex, IfxPort_InputMode_pullDown );

    //Channel B Sensor A
    //Left
    //Pin25
    IfxPort_setPinModeInput(IfxPort_P00_0.port, IfxPort_P00_0.pinIndex, IfxPort_InputMode_pullDown );

    //Channel B Sensor B
    //Left
    //Pin24
    IfxPort_setPinModeInput(IfxPort_P15_6.port, IfxPort_P15_6.pinIndex, IfxPort_InputMode_pullDown );

    //SW1(ON-OFF Button)
    IfxPort_setPinModeInput(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex, IfxPort_InputMode_pullDown );

    //SW2(CA Mode Button)
    IfxPort_setPinModeInput(IfxPort_P11_6.port, IfxPort_P11_6.pinIndex, IfxPort_InputMode_pullDown );
}

