#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "function.h"
const float pi=3.14159265359;
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
Thread Threadping;

//UART
Serial uart(D1,D0);

//led
DigitalOut ledB(LED_BLUE);
DigitalOut ledG(LED_GREEN);


//others
Serial pc(USBTX, USBRX);


//caliberate
double pwm_table0[] = {-170,-130,-90,-80,-30,-20,20,30,80,90,130,170};
double speed_table0[] = {-17.222019,-16.844446,-15.667481,-14.946926,-6.302112,-4.179810,2.849605,5.179945,14.274741,15.097291,16.715146,17.304180};//left
double pwm_table1[] = {-170,-111,-100,100,111,170};
double speed_table1[] = {-17.321571,-17.162805,-15.940337,16.097489,17.327452,17.355447};

int main(){//rotate slow need 14 holes
    uart.baud(9600);
    ledB=1;
    ledG=1;
    Timer t;
    t.start();
    car.setCalibTable(12, pwm_table0, speed_table0,6, pwm_table1, speed_table1);
    xbee.printf("start\r\n");
    car.stop();
    //testing zone


    //----
    

    xbee.printf("go straight until wall\r\n");
    car.goStraightFullSpeed(-1);
    float value=16;
    while(value>10){
        value=ping.ping_cm();
        wait(0.1);
    }
    car.stop();
    wait(0.1);
    xbee.printf("angle correction\r\n");
    car.perpendiculize(1);
    wait(0.1);
    xbee.printf("backward 8.5cm\r\n");
    car.goStraightFullSpeed(1);
    wait(0.6);
    car.stop();
    wait(0.1);
    xbee.printf("rotate 90 degree\r\n");
    car.rotateFullSpeed(92);

    xbee.printf("enter mission 1 ,go straight until wall\r\n");
    ledB=0;
    car.goStraightFullSpeed(-1);
    value=16;
    while(value>10){
        value=ping.ping_cm();
        wait(0.1);
    }
    car.stop();
    wait(0.1);

    xbee.printf("angle correction\r\n");
    car.perpendiculize(1);
    wait(0.1);

    xbee.printf("rotate -90 degree\r\n");
    car.rotateFullSpeed(-90);

    xbee.printf("parking\r\n");
    car.goStraightFullSpeed(1);
    value=11;
    while(value<40){
        value=ping.ping_cm();
        wait(0.1);
    }
    car.stop();
    wait(2);

    xbee.printf("go straight until wall\r\n");
    car.goStraightFullSpeed(-1);
    value=11;
    while(value>10){
        value=ping.ping_cm();
        wait(0.1);
    }
    car.stop();
    wait(0.1);

    xbee.printf("rotate 90 degree\r\n");
    car.rotateFullSpeed(90);
    wait(0.1);

    xbee.printf("backward 55 cm \r\n");
    car.goStraightFullSpeed(1);
    value=49;
    while(value<53){
        value=ping.ping_cm();
        wait(0.1);
    }

    car.stop();
    wait(0.1);

    xbee.printf("rotate -90 degree\r\n");
    car.rotateFullSpeed(-90);
    wait(0.1);

    xbee.printf("snapshot\r\n");
    char s[7];
    sprintf(s,"takePic");
    uart.puts(s);
    wait(1);

    xbee.printf("rotate 90 degree\r\n");
    car.rotateFullSpeed(90);
    wait(0.1);
    
    xbee.printf("backward until 75 cm \r\n");
    car.goStraightFullSpeed(1);
    value=69;
    while(value<73){
        value=ping.ping_cm();
        wait(0.1);
    }
    car.stop();
    wait(0.1);

    //get image result
    xbee.printf("image guess:");
    t.reset();
    while(t.read()<1){
        if(uart.readable()){
            char recv=uart.getc();
            xbee.putc(recv);
        }
    }
    xbee.printf("\r\n");

    xbee.printf("mission 1 end,rotate 90 degree\r\n");
    car.rotateFullSpeed(90);
    car.stop();
    wait(0.1);

    xbee.printf("moving to mission 2, go straight until wall\r\n");
    ledG=0;
    car.goStraightFullSpeed(-1);
    value=11;
    while(value>10){
        value=ping.ping_cm();
        wait(0.1);
    }
    ledB=1;
    car.stop();
    wait(0.1);

    xbee.printf("rotate -90 degree\r\n");
    car.rotateFullSpeed(-90);
    
    xbee.printf("mission 2 start,go straight 50 cm\r\n");
    car.goStraightFullSpeed(-1);
    t.reset();
    while(t.read()<2.94){}
    car.stop();
    wait(0.1);

    xbee.printf("rotate 90 degree\r\n");
    car.rotateFullSpeed(90);
    car.stop();
    wait(0.1);

    xbee.printf("angle correction\r\n");
    car.perpendiculize(1);
    wait(0.1);

    xbee.printf("backward 25 cm\r\n");
    car.goStraightFullSpeed(1);
    t.reset();
    while(t.read()<1.8){}
    car.stop();
    wait(0.1);

    xbee.printf("rotate -90 degree\r\n");
    car.rotateFullSpeed(-90);
    //determine shape
    xbee.printf("determine shape:");
    //string[4]={"right triangle,equilateral triangle,2 spike,square"};
    
    float shapeDepth=0;
    for(int i=0;i<10;i++){
        shapeDepth+=ping.ping_cm();
        wait(0.1);
    }
    shapeDepth/=10;
    if(shapeDepth<25){
        xbee.printf("square");
    }else if(shapeDepth<35){
        xbee.printf("right square");
    }else if(shapeDepth>25&&shapeDepth<30){
        xbee.printf("2 spike");
    }else{
        xbee.printf("equilateral triangle");
    }
    
    xbee.printf("(%f)\r\n",shapeDepth);
    
    //some cindition if(){}

    xbee.printf("rotate 90 degree\r\n");
    car.rotateFullSpeed(90);
    car.stop();
    wait(0.1);
    xbee.printf("go straight until wall\r\n");
    car.goStraightFullSpeed(-1);
    while(ping.ping_cm()>10){
        wait(0.1);
    }
    car.stop();
    wait(0.1);

    xbee.printf("angle correction\r\n");
    car.perpendiculize(1);
    wait(0.1);

    xbee.printf("rotate -90 degree\r\n");
    car.rotateFullSpeed(-90);
    wait(0.1);

    xbee.printf("leaving mission 2, go straight until wall\r\n");
    ledG=1;
    car.goStraightFullSpeed(-1);
    while(ping.ping_cm()>10){
        wait(0.1);
    }
    car.stop();
    wait(0.1);

    xbee.printf("rotate 90 degree\r\n");
    car.rotateFullSpeed(90);
    wait(0.1);

    xbee.printf("backward until leave cm\r\n");
    car.goStraightFullSpeed(1);
    value=149;
    while(value<150){
        value=ping.ping_cm();
        wait(0.1);
    }
    
    xbee.printf("end\r\n");
    car.stop();
    
}