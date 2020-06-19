#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

//encoder
DigitalIn pin3(D3);
DigitalIn pin4(D4);
Ticker et1,et2;
parallax_encoder encoderL(pin3,et1);
parallax_encoder encoderR(pin4,et2);

//car
PwmOut pin9(D9), pin8(D8);
BBCar car(pin8,pin9);

//xbee
Serial xbee(D12,D11);

//ping
DigitalInOut pin10(D10);
parallax_ping  ping(pin10);

//others
Serial pc(USBTX, USBRX);

//caliberate
double pwm_table0[] = {-170,-130,130,170};
double speed_table0[] = {-17.222019,-16.844446,16.715146,17.304180};//left
double pwm_table1[] = {-150,-130,130,150};
double speed_table1[] = {-17.082354,-16.940560,16.723703,17.082413};

int main(){
    car.setCalibTable(4, pwm_table0, speed_table0, 4, pwm_table1, speed_table1);
    xbee.printf("start\r\n");
    car.stop();
    /*
    encoderL.reset();
    encoderR.reset();
    double pwm=-150;
    car.servo1.pwm_value=pwm;
    car.servo1.control();
    while(encoderR.get_steps()<1){}
    Timer t;
    t.start();
    encoderR.reset();
    while(encoderR.get_steps()<10){
        xbee.printf("%d\r\n",encoderR.get_steps());
        
        //xbee.printf("ping time:%20f,ping distance:%f\r\npi",ping.ping(),ping.ping_cm()
    }
    xbee.printf("right:%f\r\n",6.5*3.14*10/t.read());
    car.stop();*/
    car.goStraightCalib(17);
    xbee.printf("pwm_value:%f,%f",car.servo0.pwm_value,car.servo1.pwm_value);
    //car.servo1.pwm_value=-138.383994;
    //car.servo1.control();
    wait(10);
    car.stop();
}