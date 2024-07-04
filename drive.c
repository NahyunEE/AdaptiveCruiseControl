#include <math.h>
#include <time.h>
#include "drive.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "uart.h"


// 모터 및 시스템 파라미터
double L = 0.5;
double R = 2;
double Kf = 0.01;
double J = 0.02;
double Kb = 0.1;
double MotorDriver_result = 12;

// PID 제어 파라미터
double Kp = 7000;
double Ki = 10;
double Kd = 3000;
double error_sum = 0;
double prev_error = 0;

//샘플링 주기
#define Samplingtime 0.001

clock_t prev_t; // 이전 시간을 저장하는 변수 선언
volatile double cur_x = 0; // 현재 x 좌표를 저장하는 변수 선언 및 초기화
volatile double cur_y = 0; // 현재 y 좌표를 저장하는 변수 선언 및 초기화

static double theta = M_PI_2; // 현재 방향 각도를 저장하는 변수 선언 및 초기화 (기본값: pi/2)

// 피드백 값을 저장하는 변수 선언 및 초기화
static double feedback1 = 0;
static double feedback2 = 0;

// 좌,우측 모터 속도를 저장하는 변수 선언 및 초기화
double lmotor_speed = 0;
double rmotor_speed = 0;

// 설정된 차량의 속도를 저장하는 변수 선언 및 초기화 (기본값: 15)
double v = 15;

void Drive_init(){
    // 좌,우측 모터 속도를 초기화할 때, 설정된 전체 속도의 1/100로 설정
    lmotor_speed = v/100;
    rmotor_speed = v/100;
}

//모터 입력에 대한 라플라스 변환을 수행하고, 그 결과를 laplace_before_result1에 저장
double laplaceTransform1(double input, double before_result) {
    double result = 0;
    result = (input - R * before_result) * Samplingtime / L + before_result;
    return result;
}

//첫 번째 변환 결과에 대한 추가 변환을 수행하고, 그 결과를 Feedback에 저장. 이는 모터의 동적 반응을 모델링하는 데 사용
double laplaceTransform2(double input, double before_result) {
    double result = 0;
    result = (input - Kf * before_result) * Samplingtime / J + before_result;
    return result;
}

//*PID 제어 함수
//[target : 목표 값] | [current : 현재 값] | [dt : 시간 간격]
double PIDControl1(double target, double current, double dt) {
    static double error_sum = 0; // 오차의 누적값을 저장하는 변수 초기화
    double error = target - current; // 현재 오차 계산
    error_sum += error; // 오차의 누적값 업데이트
    double d_error = (error - prev_error); // 오차 변화율 계산
    prev_error = error; // 이전 오차 값 업데이트

    // PID 제어 입력 계산
    double control_input = Kp * error + Ki * error_sum + Kd * d_error;

    // 제어 입력이 상한값을 초과하면 상한값으로 설정
    if (control_input > 1000) {
        control_input = 1000;
    }
    // 제어 입력이 하한값 미만이면 하한값으로 설정
    if (control_input < 0) {
        control_input = 0;
    }
    return control_input;
}


double PIDControl2(double target, double current,double dt) {
    static double error_sum = 0; // 오차의 누적값을 저장하는 변수 초기화
    double error = target - current; // 현재 오차 계산
    error_sum += error; // 오차의 누적값 업데이트
    double d_error = (error - prev_error); // 오차 변화율 계산
    prev_error = error; // 이전 오차 값 업데이트

    // PID 제어 입력 계산
    double control_input = Kp * error + Ki * error_sum + Kd * d_error;

    // 제어 입력이 상한값을 초과하면 상한값으로 설정
    if (control_input > 1000) {
        control_input = 1000;
    }
    // 제어 입력이 하한값 미만이면 하한값으로 설정
    if (control_input < 0) {
        control_input = 0;
    }
    return control_input;
}

// *옵저버 함수: 제어 입력을 통해 피드백을 추정하는 함수
double Observer1(double control_input){
    static double laplace_before_result1 = 0; // 라플라스 변환 이전 결과 초기화
    static double Feedback = 0; // 피드백 초기화

    //*입력된 control_input이 12보다 크면 12로 제한하고, 0보다 작으면 0으로 설정. 이는 모터 드라이버의 입력 전압 범위를 고려함
    // 제어 입력이 상한값을 초과하면 상한값으로 설정
    if (control_input > 12) {
        control_input = 12;
    }
    // 제어 입력이 하한값 미만이면 하한값으로 설정
    if (control_input < 0) {
        control_input = 0;
    }

    //모터 입력 계산
    //제어 입력에서 피드백을 고려한 값을 계산
    //Kb(역기전력상수)를 고려하여 실제 모터에 입력되는 전압으로 조절.
    double Motor_input = control_input - Feedback * Kb;

    // 라플라스 변환 결과 계산 및 업데이트
    laplace_before_result1 = laplaceTransform1(control_input, laplace_before_result1);

    // 피드백 계산
    Feedback = laplaceTransform2(laplace_before_result1, Feedback);

    // 추정된 피드백 반환
    return Feedback;
}
double Observer2(double control_input){
    static double laplace_before_result1 = 0; // 라플라스 변환 이전 결과 초기화
    static double Feedback = 0; // 피드백 초기화

    // 제어 입력이 상한값을 초과하면 상한값으로 설정
    if (control_input > 12) {
        control_input = 12;
    }
    // 제어 입력이 하한값 미만이면 하한값으로 설정
    if (control_input < 0) {
        control_input = 0;
    }
    // 모터 입력 계산
    double Motor_input = control_input - Feedback * Kb;
    // 라플라스 변환 결과 계산 및 업데이트
    laplace_before_result1 = laplaceTransform1(control_input, laplace_before_result1);
    // 피드백 계산
    Feedback = laplaceTransform2(laplace_before_result1, Feedback);
    // 추정된 피드백 반환
    return Feedback;
}

// 두 지점 사이의 거리 계산
double cal_distance(double cur_x, double cur_y, double next_x, double next_y) {
    return sqrt(pow((cur_x - next_x), 2) + pow((cur_y - next_y), 2));
}

// 두 지점 사이의 각도 계산
double cal_theta(double cur_x, double cur_y, double next_x, double next_y) {
    return atan2((next_y - cur_y), (next_x - cur_x));
}

// 좌우 모터 속도를 계산하여 설정하는 함수
void lrmotor_speed(double R, double l, double w) {
    static volatile double temp_l = 0; // 좌 모터 속도 임시 저장 변수 초기화
    static volatile double temp_r = 0; // 우 모터 속도 임시 저장 변수 초기화

    // 좌,우 모터 속도 계산
    lmotor_speed = w * (R - l / 2) / 10;
    rmotor_speed = w * (R + l / 2) / 10;

    // 속도 임시 저장
    temp_l = lmotor_speed;
    temp_r = rmotor_speed;
}

//현재 위치, 방향, 속도 등을 기반으로 순수 추종 알고리즘을 수행하여 다음 경로의 방향을 추정
double pure_pursuit_algorism(double cur_x, double cur_y, double cur_theta, double next_x, double next_y,double v_mili){
    volatile double dist, delta_theta, w, R;

    // 현재 위치와 다음 위치 간의 거리 계산
    dist = cal_distance(cur_x, cur_y, next_x, next_y);

    // 현재 각도와 다음 위치를 향한 각도 차이 계산
    delta_theta = cal_theta(cur_x, cur_y, next_x, next_y) - cur_theta;

    // 타원 경로 추정을 위한 반지름 계산
    R = dist / sin(delta_theta);

    // 각속도(w) 계산
    w = v_mili / R;

    // 좌우 모터 속도 조절
    lrmotor_speed(R, 20, w);

    // 각속도 반환
    return w;
}

// 현재 위치 및 각도를 업데이트하는 함수
// 주어진 속도와 방향 변경량을 바탕으로 차량의 새로운 위치를 계산
void update_current(double v,double w,double delta_t){
    double delta_theta;
    double delta_x;
    double delta_y;

    // 각도 변화량 계산
    delta_theta = w * delta_t;

    // 현재 각도 업데이트
    theta += delta_theta;

    // 각도가 PI를 넘어가면 범위 내로 조정
    if (theta > M_PI) {
        theta = -2 * M_PI - theta;
    }

    // 각도가 -PI를 넘어가면 범위 내로 조정
    if (theta < -M_PI) {
        theta = 2 * M_PI + theta;
    }

    // 이동량 계산
    delta_x = v * delta_t * cos(theta);
    delta_y = v * delta_t * sin(theta);

    // 현재 위치 업데이트
    cur_x += delta_x;
    cur_y += delta_y;
}


//[t: 베지어 곡선의 매개변수, 0과 1 사이의 값으로, 곡선의 시작점에서 끝점까지의 위치] | [a: 곡선의 형태를 결정하는 외부에서 제공된 스케일링 인자]
// t에 대한 호의 길이 함수를 계산하는 함수
double arcLengthFunction(double t, double a) {
    // x,y 좌표에 대한 t의 도함수 계산
    double dx_dt = 6 * (1 - t) * t * (-2 * a);
    double dy_dt = 3 * (1 - t) * (1 - t) * a  + 3 * t * t * a;

    // t에 대한 호의 길이 함수 값 계산 및 반환
    return sqrt(dx_dt * dx_dt + dy_dt * dy_dt);
}

// 주어진 t까지의 호의 길이를 적분하여 계산하는 함수
double integrateArcLength(double t, double a) {
    double deltaT = t / numIntervals;  // 각 구간의 너비 계산
    double sum = 0.5 * (arcLengthFunction(0, a) + arcLengthFunction(t, a));  // 초기값 설정

    // 각 구간에서의 호의 길이를 더하여 적분 계산
    for (int i = 1; i < numIntervals; i++) {
        sum += arcLengthFunction(i * deltaT, a);
    }

    // 적분 결과 반환
    return sum * deltaT;
}

// 뉴튼-랩슨 방법을 사용하여 길이 L이 주어졌을 때 t를 찾는 함수
double findTForGivenLength(double L, double a,double initialGuess) {
    double t = initialGuess;  // 초기 추정값으로 t 설정
    double s = integrateArcLength(t, a);  // 초기 추정값에 대한 호의 길이 계산
    int iteration = 0;  // 반복 횟수 초기화

    // 허용 오차보다 호의 길이와 주어진 길이의 차이가 크고 최대 반복 횟수에 도달하기 전까지 반복
    while (abs(s - L) > tolerance && iteration < maxIterations) {
        double ds_dt = arcLengthFunction(t, a);  // t에 대한 호의 길이 함수의 도함수 계산
        double tNew = t - (s - L) / ds_dt;  // 새로운 t 값 계산
        t = tNew;  // t 값을 업데이트
        s = integrateArcLength(t, a);  // 새로운 t 값에 대한 호의 길이 계산
        iteration++;  // 반복 횟수 증가
    }
    return t;  // 찾은 t 값 반환
}

// 주어진 t에 대한 cubic Bezier 곡선의 x 좌표를 계산하는 함수
double bezierX(double t, double a) {
    //두 번째, 세 번째 제어점의 x좌표
    double x2 = -2 * a;
    double x3 = -2 * a;

    // cubic Bezier 곡선의 x 좌표 계산
    return 3 * (1 - t) * pow(t, 2) * x2 + pow(t, 3) * x3;
}

// 주어진 t에 대한 cubic Bezier 곡선의 y 좌표를 계산하는 함수
double bezierY(double t, double a) {
    //3개 제어점의 y좌표
    double y1 = a;
    double y2 = a;
    double y3 = 2 * a;

    // cubic Bezier 곡선의 y 좌표 계산
    return 3 * pow(1 - t, 2) * t * y1 + 3 * (1 - t) * pow(t, 2) * y2 + pow(t, 3) * y3;
}

// 베지어 곡선을 이용하여 주어진 매개변수 t에 대한 위치 (x, y)를 계산하며, 주로 경로 계획에서 스무스하고 정확한 경로를 생성하는 데 사용됨.
void Drive_main(double a, int flag) {
    static int drive_flag = 0;
    static double L = 0;  // 이동 거리 변수 선언 및 초기화
    static double t = 0;  // 시간 변수 선언 및 초기화
    if(drive_flag || flag==2){
        drive_flag = 1;
        double v_mili = v/10;
        clock_t delta_t = 1;

        //매개변수 t가 1.2미만일 때, 즉 곡선의 끝에 도달하지 않았을 때
        if(t < 1.2){
            //findTForGivenLength 함수를 사용하여 경로에 따른 새로운 t 값을 계산
            t = findTForGivenLength(L + v_mili * delta_t + v_mili, a, t);

            //bezierX와 bezierY 함수를 통해 t 에 해당하는 베지어 곡선 상의 위치 (next_x, next_y)를 계산
            double next_x = bezierX(t, a);
            double next_y = bezierY(t, a);

            //pure_pursuit_algorism 함수로 순수 추종 알고리즘을 실행하여 차량의 각속도 w를 계산하고, 이를 통해 차량이 다음 위치로 향하도록 함
            double w = pure_pursuit_algorism(cur_x, cur_y, theta, next_x, next_y, v_mili);

            // 이동 거리 업데이트
            L += delta_t * v_mili;

            //차량의 현재 위치를 update_current 함수를 사용하여 업데이트
            update_current(v_mili, w, delta_t);
        }
        //t가 1.2이상이면, 즉 베지어 곡선의 끝에 도달했을 때 실행
        else{
            //L = 0, cur_x = 0, cur_y = 0, theta = M_PI_2 : 위치와 관련된 변수들을 초기화하고 theta는 π/2로 재설정하여 차량의 방향을 초기화
            L = 0;
            cur_x = 0;
            cur_y = 0;
            theta = M_PI_2;

            // 드라이브 플래그 설정
            drive_flag = 1;

            // 왼쪽 및 오른쪽 모터 속도 설정
            lmotor_speed = v / 100;
            rmotor_speed = v / 100;
        }
    }
}

int PID_main(){
    // 왼쪽,오른쪽 모터에 대한 PID 제어를 수행하여 제어량을 계산하고 저장
    volatile double control1 = PIDControl1(lmotor_speed, feedback1, 0.01);
    volatile double control2 = PIDControl2(rmotor_speed, feedback2, 0.01);

    volatile double temp1 = 0;  // 임시 변수1 선언 및 초기화
    volatile double temp2 = 0;  // 임시 변수2 선언 및 초기화

    // 피드백 변수에 왼쪽,오른쪽 모터 속도를 할당
    feedback1 = L_w;
    feedback2 = R_w;

    volatile double temp3 = L_w;  // 임시 변수3 선언 및 왼쪽 모터 속도 할당
    volatile double temp4 = R_w;  // 임시 변수4 선언 및 오른쪽 모터 속도 할당
    temp1 = lmotor_speed;  // 임시 변수1에 왼쪽 모터 속도 할당
    temp2 = rmotor_speed;  // 임시 변수2에 오른쪽 모터 속도 할당

    // 왼쪽,오른쪽 모터에 제어 입력 값을 적용하여 PWM 신호의 듀티 사이클 설정
    setDutyCycleA((int)(20000 + control1 * 30));
    setDutyCycleB((int)(20000 + control2 * 30));

    return 0;
}

void Speed_update(){
    //좌,우측 모터 속도 업데이트
    getLeftSpeed();
    getRightSpeed();
}
