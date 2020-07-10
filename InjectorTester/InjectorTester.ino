//BOTONES RPM
const int b_800rpm = 7;
const int b_1500rpm = 8;
const int b_3600rpm = 9;
const int b_5000rpm = 10;
//SALIR
const int b_salir = A5;
//MANTENER PRESIONADO
const int b_consflujo = 12;
int eb_consflujo;
//LED DE ENCENDIDO
const int LEDReady = 13;
//LED RPMS EN FUNCIONAMIENTO
const int LedRPM = 11;
//LEDS RPM
const int LED800rpm = 3;
const int LED1500rpm = 4;
const int LED3600rpm = 5;
const int LED5000rpm = 6;
//INYECTOR
const int inject1 = 2;    //pin de injeccion pin9
//ESTADO INYECTOR Y TIEMPO
int inject_State = LOW;
unsigned long previousMillis = 0; 
//ESTADOS DE BOTONES
int eb_800rpm;
int eb_1500rpm;
int eb_3600rpm;
int eb_5000rpm;
//INTERRUMPIR FUNCIONAMIENTO
int eb_salir = 0;
//TIMING BOTONES
unsigned long time_now = 0;
unsigned long time_rpm = 0;
const int timer = 10000; //TIMING 10 SEGUNDOS
int botonApretado;
void setup() {
    pinMode(b_800rpm, INPUT);
    pinMode(b_1500rpm, INPUT);
    pinMode(b_3600rpm, INPUT);
    pinMode(b_5000rpm, INPUT);
    pinMode(LED800rpm, OUTPUT);
    pinMode(LED1500rpm, OUTPUT);
    pinMode(LED3600rpm, OUTPUT);
    pinMode(LED5000rpm, OUTPUT);
    pinMode(LEDReady, OUTPUT);
    pinMode(b_salir, INPUT);  
    pinMode(b_consflujo, INPUT);
    pinMode(LedRPM, OUTPUT);
    pinMode(inject1, OUTPUT);
    digitalWrite(LEDReady, HIGH);
    Serial.begin(9600);
}
void loop() {  
    eb_800rpm = digitalRead(b_800rpm);
    eb_1500rpm = digitalRead(b_1500rpm);
    eb_3600rpm = digitalRead(b_3600rpm);
    eb_5000rpm = digitalRead(b_5000rpm);
    eb_consflujo = digitalRead(b_consflujo);
    if(eb_800rpm == HIGH && eb_1500rpm == LOW && eb_3600rpm == LOW && eb_5000rpm == LOW){
        botonApretado = 1;
    }
    else if(eb_800rpm == LOW && eb_1500rpm == HIGH && eb_3600rpm == LOW && eb_5000rpm == LOW){
        botonApretado = 2;
    }
    else if(eb_800rpm == LOW && eb_1500rpm == LOW && eb_3600rpm == HIGH && eb_5000rpm == LOW){
        botonApretado = 3;
    }
    else if(eb_800rpm == LOW && eb_1500rpm == LOW && eb_3600rpm == LOW && eb_5000rpm == HIGH){
        botonApretado = 4;
    }
    else if(eb_800rpm == LOW && eb_1500rpm == LOW && eb_3600rpm == LOW && eb_5000rpm == LOW && eb_consflujo == HIGH){
        botonApretado = 5;
    }
    else if(eb_800rpm == LOW && eb_1500rpm == LOW && eb_3600rpm == LOW && eb_5000rpm == LOW && eb_consflujo == LOW){
        botonApretado = 6;
    }
    switch(botonApretado){
        case 1: 
            //800 RPM
            time_now = millis();
            time_rpm = time_now;
            do{
                pulse(76, 61, LED800rpm);
                eb_salir = digitalRead(b_salir);
                if(eb_salir == HIGH){
                    digitalWrite(LedRPM, LOW);
                    digitalWrite(LED800rpm, LOW);
                    break;
                }
            }
            while(millis() < (time_rpm + timer));
            botonApretado = restart(eb_800rpm, LED800rpm);
        break;
        case 2:
            //3600 RPM
            time_now = millis();
            time_rpm = time_now;
            do{
                pulse(40, 32, LED1500rpm);
                eb_salir = digitalRead(b_salir);
                if(eb_salir == HIGH){
                    digitalWrite(LedRPM, LOW);
                    digitalWrite(LED1500rpm, LOW);
                    break;
                }
            }
            while(millis() < (time_rpm + timer));
            botonApretado = restart(eb_1500rpm, LED1500rpm);
        break;
        case 3:
            //3600 RPM
            time_now = millis();
            time_rpm = time_now;
            do{
                pulse(16.6, 13.28, LED3600rpm);
                eb_salir = digitalRead(b_salir);
                if(eb_salir == HIGH){
                    digitalWrite(LedRPM, LOW);
                    digitalWrite(LED3600rpm, LOW);
                    break;
                }
            }
          while(millis() < (time_rpm + timer));
            botonApretado = restart(eb_3600rpm, LED3600rpm);
        break;
        case 4:
            //5000 RPM
          time_now = millis();
            time_rpm = time_now;
            do{
                pulse(10.52, 8.4, LED5000rpm);
                eb_salir = digitalRead(b_salir);
                if(eb_salir == HIGH){
                    digitalWrite(LedRPM, LOW);
                    digitalWrite(LED5000rpm, LOW);
                    break;
                }
            }
            while(millis() < (time_rpm + timer));
            botonApretado = restart(eb_5000rpm, LED5000rpm);
        break;
        case 5:
            digitalWrite(inject1, HIGH);
        break;
        case 6:
            digitalWrite(inject1, LOW);
        break;
    };
} 

void pulse(float OnTime, float OffTime, int led) {
    delay(300);
    digitalWrite(LedRPM, HIGH);
    digitalWrite(led, HIGH);
    unsigned long currentMillis = millis(); 
    if((inject_State == HIGH) && (currentMillis - previousMillis >= OnTime)){
      inject_State = LOW;  //APAGADO
        previousMillis = currentMillis;  //RECORDANDO TIEMPO
        digitalWrite(inject1, inject_State); //ACTUALIZA EL INJEC1 ACTUAL
    }
    else if ((inject_State == LOW) && (currentMillis - previousMillis >= OffTime)){
        inject_State = HIGH;  // lo encuende
        previousMillis = currentMillis;   // Rercuerda el tiempo
        digitalWrite(inject1, inject_State); //ACTUALIZA EL INJEC1 ACTUAL
    }  
}

int restart(int led1, int led2){
    digitalWrite(led1, LOW);
    digitalWrite(LedRPM, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(inject1, LOW);
    return 0;
}
