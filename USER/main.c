#include "AllHeader.h"

#define UPLOAD_DATA 2  //1:????????? 2:????????
                       
#define MOTOR_TYPE 1   //1:520?? 2:310?? 3:????TT?? 4:TT?????? 5:L?520??

uint8_t times = 0;
uint8_t upload_counter = 0;  // ???????

void Car_Move(void)
{
    static uint8_t state = 0;
    switch(state)
    {
        case 0:
        control_speed(300,300,300,300);//??
        break;
        
        case 1:
        control_speed(-300,-300,-300,-300);//??
        break;
        
        case 2:
        control_speed(300,300,-600,-600);//??
        break;
        
        case 3:
        control_speed(-600,-600,300,300);//??
        break;
        
        case 4:
        control_speed(0,0,0,0);//??
        break;
    }
    state++;
    if(state>4)state=0;
}

void Car_Move_PWM(void)
{
    static uint8_t state = 0;
    switch(state)
    {
        case 0:
        control_pwm(1500,1500,1500,1500);//??
        break;
        
        case 1:
        control_pwm(-1500,-1500,-1500,-1500);//??
        break;
        
        case 2:
        control_pwm(1200,1200,-1500,-1500);//??
        break;
        
        case 3:
        control_pwm(-1500,-1500,1200,1200);//??
        break;
        
        case 4:
        control_pwm(0,0,0,0);//??
        break;
    }
    state++;
    if(state>4)state=0;
}

int main(void)
{	
    bsp_init();
    
    TIM3_Init();
    
    // ????iic?????
    IIC_Motor_Init();
    printf("pelase wait...\r\n");
    control_pwm(0,0,0,0);
    delay_ms(1000);  // ?????????

    #if MOTOR_TYPE == 1
    Set_motor_type(1);//??????
    delay_ms(100);
    Set_Pluse_Phase(30);//?????
    delay_ms(100);
    Set_Pluse_line(11);//?????
    delay_ms(100);
    Set_Wheel_dis(67.00);//??????
    delay_ms(100);
    Set_motor_deadzone(1900);//??????
    delay_ms(100);
    
    #elif MOTOR_TYPE == 2
    Set_motor_type(2);
    delay_ms(100);
    Set_Pluse_Phase(20);
    delay_ms(100);
    Set_Pluse_line(13);
    delay_ms(100);
    Set_Wheel_dis(48.00);
    delay_ms(100);
    Set_motor_deadzone(1600);
    delay_ms(100);
    
    #elif MOTOR_TYPE == 3
    Set_motor_type(3);
    delay_ms(100);
    Set_Pluse_Phase(45);
    delay_ms(100);
    Set_Pluse_line(13);
    delay_ms(100);
    Set_Wheel_dis(68.00);
    delay_ms(100);
    Set_motor_deadzone(1250);
    delay_ms(100);
    
    #elif MOTOR_TYPE == 4
    Set_motor_type(4);
    delay_ms(100);
    Set_Pluse_Phase(48);
    delay_ms(100);
    Set_motor_deadzone(1000);
    delay_ms(100);
    
    #elif MOTOR_TYPE == 5
    Set_motor_type(1);
    delay_ms(100);
    Set_Pluse_Phase(40);
    delay_ms(100);
    Set_Pluse_line(11);
    delay_ms(100);
    Set_Wheel_dis(67.00);
    delay_ms(100);
    Set_motor_deadzone(1900);
    delay_ms(100);
    #endif

while(1)
{
    // ??????(250 * 10ms = 2.5?)
    if(times >= 250) 
    {
        #if MOTOR_TYPE == 4
        Car_Move_PWM();
        #else
        Car_Move();
        #endif
        times = 0;
    }

    // ?20ms??????(10ms*2)
    if(upload_counter >= 2)  
    {
        #if UPLOAD_DATA == 2
        // ????10ms????(??????)
        Read_10_Enconder();
        printf("M1:%d\t M2:%d\t M3:%d\t M4:%d\r\n",
               Encoder_Offset[0], Encoder_Offset[1], 
               Encoder_Offset[2], Encoder_Offset[3]);
        #else
        // UPLOAD_DATA == 1??????(mm/s),?????
        Read_ALL_Enconder();  // ?????????????
        
        // ???????????????????
        printf("M1:%d\t M2:%d\t M3:%d\t M4:%d\r\n",
               Current_Speed[0], Current_Speed[1], 
               Current_Speed[2], Current_Speed[3]);
        #endif
        
        upload_counter = 0;
    }
    
    // ?????10ms??
    delay_ms(10);
    times++;
    upload_counter++;
}
}