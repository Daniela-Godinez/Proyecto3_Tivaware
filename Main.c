#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"

#define XTAL 16000000

//**************************************************************************************************************
// Variables Globales
//**************************************************************************************************************

char charIn = '0';
uint8_t modo = 0;
uint8_t estado = 0;
uint8_t segmento = 0;
uint8_t contador_HEX = 0;
int8_t contador_LED = 0;
uint32_t ui32Period;
uint32_t PeriodB;
uint8_t DIP1 = 0;
uint8_t DIP2 = 0;
uint8_t secuencia1 = 0;
uint8_t secuencia2 = 0;
uint32_t ADC_Lecture;
uint8_t conversion = 0;
uint8_t unidades = 1;
uint8_t decenas = 0;

//**************************************************************************************************************
// Prototipos de Función
//**************************************************************************************************************
void IOConfig (void);
void InitUART(void);
void UART0ReadIntHandler(void);
void Timer0AIntHandler(void);
void Timer1AIntHandler(void);
void GPIOIntHandler (void);
unsigned short map(uint32_t val, uint32_t in_min, uint32_t in_max, unsigned short out_min, unsigned short out_max);
//**************************************************************************************************************
// Código Principal
//**************************************************************************************************************
int main(void){

    // CONFIGURACION
    IOConfig();

    //**********************************************************************************************************
    // LOOP
    //**********************************************************************************************************
    while (1){

        // DIP 1 - ON/OFF
        if ((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1)&0b00000010)==0b00000010){
            DIP1 = 1;
        }else if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1)&0b00000010)==0b00000000){
            DIP1 = 0;
        }
        // DIP 2 - ON/OFF
        if ((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2)&0b00000100)==0b00000100){
            DIP2 = 1;
        }else if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2)&0b00000100)==0b00000000){
            DIP2 = 0;
        }
        // MODOS
        if ((DIP1 == 0)&&(DIP2 == 0)){ //MODO 1 - DIPS OFF
            modo = 1;
        }else if ((DIP1 == 0)&&(DIP2 == 1)){ //MODO 2 - DIP1 OFF y DIP2 ON
            modo = 2;
        }else if ((DIP1 == 1)&&(DIP2 == 0)){ //MODO 3 - DIP1 ON y DIP2 OFF
            modo = 3;
        }else if ((DIP1 == 1)&&(DIP2 == 1)){ //MODO 4 - DIPS ON
            modo = 4;
        }

        //CASO 1 - MODO 1
        if (modo == 1){
            //LED 1
            if (charIn == 'a'){
                charIn = 'x';
                // Cambiar el estado del LED 1
                if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4)){
                    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
                }else{
                    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0b00010000);
                }
            }
            //LED 2
            if (charIn == 'b'){
                charIn = 'x';
                if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5)){
                    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
                }else{
                    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0b00100000);
                }
            }
            //LED 3
            if (charIn == 'c'){
                charIn = 'x';
                if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6)){
                    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
                }else{
                    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0b01000000);
                }
            }
            //LED 4
            if (charIn == 'd'){
                charIn = 'x';
                if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7)){
                    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
                }else{
                    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0b10000000);
                }
            }

            //DISPLAY
            if (charIn == '0'){
                charIn = 'x';
                segmento = 0;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00111111);
            }

            if (charIn == '1'){
                charIn = 'x';
                segmento = 1;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000110);
            }

            if (charIn == '2'){
                charIn = 'x';
                segmento = 2;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01011011);
            }

            if (charIn == '3'){
                charIn = 'x';
                segmento = 3;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01001111);
            }

            if (charIn == '4'){
                charIn = 'x';
                segmento = 4;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01100110);
            }

            if (charIn == '5'){
                charIn = 'x';
                segmento = 5;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01101101);
            }

            if (charIn == '6'){
                charIn = 'x';
                segmento = 6;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01111101);
            }

            if (charIn == '7'){
                charIn = 'x';
                segmento = 7;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000111);
            }

            if (charIn == '8'){
                charIn = 'x';
                segmento = 8;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01111111);
            }

            if (charIn == '9'){
                charIn = 'x';
                segmento = 9;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01101111);
            }else{
                charIn = charIn;
            }

            if (estado == 1){
                if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4)){
                    UARTCharPut(UART0_BASE, 'L');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'O');
                    UARTCharPut(UART0_BASE, 'N');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }else if (!GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4)){
                    UARTCharPut(UART0_BASE, 'L');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'O');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5)){
                    UARTCharPut(UART0_BASE, 'L');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '2');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'O');
                    UARTCharPut(UART0_BASE, 'N');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }else if (!GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5)){
                    UARTCharPut(UART0_BASE, 'L');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '2');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'O');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6)){
                    UARTCharPut(UART0_BASE, 'L');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '3');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'O');
                    UARTCharPut(UART0_BASE, 'N');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }else if (!GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6)){
                    UARTCharPut(UART0_BASE, 'L');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '3');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'O');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7)){
                    UARTCharPut(UART0_BASE, 'L');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '4');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'O');
                    UARTCharPut(UART0_BASE, 'N');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }else if (!GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7)){
                    UARTCharPut(UART0_BASE, 'L');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '4');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'O');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 0){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '0');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 1){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 2){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '2');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 3){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '3');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 4){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '4');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 5){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '5');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 6){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '6');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 7){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '7');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 8){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '8');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (segmento == 9){
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 's');
                    UARTCharPut(UART0_BASE, 'p');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'y');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '9');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }
                estado = 0;
            }
        }

        // CASO 2 - MODO 2

        else if (modo == 2){

            // CONTADOR HEX
            if (contador_HEX == 0){
                segmento = 0;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00111111);
            }

            if (contador_HEX == 1){
                segmento = 1;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000110);
            }

            if (contador_HEX == 2){
                segmento = 2;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01011011);
            }

            if (contador_HEX == 3){
                segmento = 3;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01001111);
            }

            if (contador_HEX == 4){
                segmento = 4;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01100110);
            }

            if (contador_HEX == 5){
                segmento = 5;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01101101);
            }

            if (contador_HEX == 6){
                segmento = 6;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01111101);
            }

            if (contador_HEX == 7){
                segmento = 7;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000111);
            }

            if (contador_HEX == 8){
                segmento = 8;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01111111);
            }

            if (contador_HEX == 9){
                segmento = 9;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01101111);
            }

            if (contador_HEX == 10){
                segmento = 9;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01110111);
            }

            if (contador_HEX == 11){
                segmento = 9;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01111100);
            }

            if (contador_HEX == 12){
                segmento = 9;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00111001);
            }

            if (contador_HEX == 13){
                segmento = 9;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01011110);
            }

            if (contador_HEX == 14){
                segmento = 9;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01111001);
            }

            if (contador_HEX == 15){
                segmento = 9;
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01110001);
            }

            // CONTADOR LEDs
            if (charIn == '+'){
                charIn = 'x';
                contador_LED++;
                if (contador_LED == 16){
                    contador_LED = 0;
                }
            }

            if (charIn == '-'){
                charIn = 'x';
                contador_LED--;
                if (contador_LED == -1){
                    contador_LED = 15;
                }
            }

            // Mostrar contador de 4 bits en los LEDs
            if (contador_LED == 0){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x00);
            }else if (contador_LED == 1){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x10);
            }else if (contador_LED == 2){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x20);
            }else if (contador_LED == 3){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x30);
            }else if (contador_LED == 4){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x40);
            }else if (contador_LED == 5){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x50);
            }else if (contador_LED == 6){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x60);
            }else if (contador_LED == 7){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x70);
            }else if (contador_LED == 8){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x80);
            }else if (contador_LED == 9){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x90);
            }else if (contador_LED == 10){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0xA0);
            }else if (contador_LED == 11){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0xB0);
            }else if (contador_LED == 12){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0xC0);
            }else if (contador_LED == 13){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0xD0);
            }else if (contador_LED == 14){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0xE0);
            }else if (contador_LED == 15){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0xF0);
            }

            // Enviar el estado de ambos contadores por UART
            if (estado == 1){
                if (contador_HEX == 0){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '0');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 1){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 2){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '2');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 3){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '3');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 4){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '4');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 5){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '5');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 6){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '6');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 7){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '7');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 8){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '8');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 9){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '9');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 10){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'A');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 11){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'B');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 12){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 13){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'D');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 14){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_HEX == 15){
                    UARTCharPut(UART0_BASE, 'H');
                    UARTCharPut(UART0_BASE, 'E');
                    UARTCharPut(UART0_BASE, 'X');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 0){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '0');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 1){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 2){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '2');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 3){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '3');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 4){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '4');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 5){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '5');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 6){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '6');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 7){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '7');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 8){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '8');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 9){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '9');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 10){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, '0');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 11){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 12){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, '2');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 13){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, '3');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 14){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, '4');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }

                if (contador_LED == 15){
                    UARTCharPut(UART0_BASE, 'C');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'n');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, 'o');
                    UARTCharPut(UART0_BASE, 'r');
                    UARTCharPut(UART0_BASE, ':');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, '1');
                    UARTCharPut(UART0_BASE, '5');
                    UARTCharPut(UART0_BASE, 10);
                    UARTCharPut(UART0_BASE, 13);
                }
                estado = 0;
            }
        }

        // CASO 3 - MODO 3

        else if (modo == 3){

            // ANIMACION DISPLAY

            if (secuencia1 == 0){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00010001);
            }

            if (secuencia1 == 1){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b10000010);
            }

            if (secuencia1 == 2){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000101);
            }

            if (secuencia1 == 3){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b10001000);
            }

            if (secuencia1 == 4){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01000001);
            }

            if (secuencia1 == 5){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b10100000);
            }

            // ANIMACION LEDs

            if (secuencia2 == 0){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x00);
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0b00010000);
            }

            if (secuencia2 == 2){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x00);
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0b00100000);
            }

            if (secuencia2 == 4){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x00);
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0b01000000);
            }

            if (secuencia2 == 6){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x00);
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0b10000000);
            }
        }

        // CASO 4 - MODO 4

        else if (modo == 4){
            // Iniciamos conversión de ADC
            ADCProcessorTrigger(ADC0_BASE, 3);
            // Esperamos a que termine la conversión
            while(!ADCIntStatus(ADC0_BASE, 3, false)){}
            // Limpiamos bandera de interrupción
            ADCIntClear(ADC0_BASE, 3);
            // Guardamos el valor de la lectura en una variable
            ADCSequenceDataGet(ADC0_BASE, 3, &ADC_Lecture);
            // Mapeo del valor ADC
            conversion = map(ADC_Lecture, 0, 4096, 0, 100);
            // Obtener decenas y unidades de valor ADC 0-99
            decenas = (conversion/10);
            unidades = (conversion-(decenas*10));
            // Mostrar unidades en display de 7 segmento
            if (unidades == 0){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00111111);
            }
            if (unidades == 1){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000110);
            }
            if (unidades == 2){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01011011);
            }
            if (unidades == 3){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01001111);
            }
            if (unidades == 4){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01100110);
            }
            if (unidades == 5){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01101101);
            }
            if (unidades == 6){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01111101);
            }
            if (unidades == 7){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b00000111);
            }
            if (unidades == 8){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01111111);
            }
            if (unidades == 9){
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0x00);
                GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, 0b01101111);
            }
            // Mostrar decenas en 4 LEDs en binario
            if (decenas == 0){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x00);
            }
            if (decenas == 1){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x10);
            }
            if (decenas == 2){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x20);
            }
            if (decenas == 3){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x30);
            }
            if (decenas == 4){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x40);
            }
            if (decenas == 5){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x50);
            }
            if (decenas == 6){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x60);
            }
            if (decenas == 7){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x70);
            }
            if (decenas == 8){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x80);
            }
            if (decenas == 9){
                GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, 0x90);
            }
        }
    }
}
//**************************************************************************************************************
// CONFIGURACION
//**************************************************************************************************************
void IOConfig (void)
{
    // Se setea oscilador externo de 16MHz
    SysCtlClockSet(
    SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Se inicializa la comunicación UART
    InitUART();

    // Se habilita el puerto F, B, C, D y E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Se establecen como salidas los pines deseados
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);

    // Se establecen como entradas los pines deseados
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4);

    // Configuracion ADC
    // Habilitamos el módulo ADC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    // Habilitamos PORTE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    // Configuramos PE3 como entrada analógica
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    // Configuramos el muestreador que utilizará al módulo ADC (1 muestra por conversión)
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    // Configuramos secuencia de muestreo del ADC
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    // Habilitamos la secuencia de muestreo del ADC
    ADCSequenceEnable(ADC0_BASE, 3);
    // Borramos la interrupcion del ADC
    ADCIntClear(ADC0_BASE, 3);

    // Se habilita el temporizador 0 y 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    // Habilitar interrupciones
    IntEnable(INT_UART0);
    // Habilitar el FIFO en 1/8 datos recibidos
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    // Habilitar interrupción de recepción de UART0
    UARTIntEnable(UART0_BASE, UART_INT_RX);
    // Habilitar interrupción de recepción de GPIO

    // Habilitamos interrupciones para Puerto D y E
    IntEnable(INT_GPIOD);
    IntEnable(INT_GPIOE);
    // Interrupcion en flanco de subida para pin 6 del Puerto D
    GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_INT_PIN_6, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_INT_PIN_6);
    // Interrupcion en flanco de subida para pin 4 del Puerto E
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_INT_PIN_4, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_4);


    // Configuración del Timer 0 como temporizador periodico
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // Configuración del Timer 1 como temporizador periodico
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    // Se calcula el período para los temporizadores
    ui32Period = (SysCtlClockGet());
    PeriodB = (SysCtlClockGet() / 10);

    // Establecer el periodo de los temporizadores
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);
    TimerLoadSet(TIMER1_BASE, TIMER_A, PeriodB - 1);

    // Se habilita la interrupción por los temporizadores
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER1A);

    // Se establece que exista la interrupción por Timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // Se habilitan los temporizadores
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER1_BASE, TIMER_A);

    // Habilitar interrupciones globales
    IntMasterEnable();

}

//**************************************************************************************************************
// FUNCIONES
//**************************************************************************************************************
void InitUART(void){
    // Habilitar Puerto A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Habilitar modulo UART 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Pines UART perifericos
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configuracion UART
    UARTConfigSetExpClk(
            UART0_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
}
// Mapeo de numeros
unsigned short map(uint32_t x, uint32_t x0, uint32_t x1,
            unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}
//**************************************************************************************************************
// INTERRUPCIONES
//**************************************************************************************************************
void UART0ReadIntHandler(void){
    // Boramos interrupcion UART
    UARTIntClear(UART0_BASE, UART_INT_RX);

    // Guardamos char recibido en una variable
    charIn = UARTCharGet(UART0_BASE);

    // Imprimimos valor recibido en consola
    UARTCharPut(UART0_BASE, 'U');
    UARTCharPut(UART0_BASE, 's');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'r');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'i');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 'p');
    UARTCharPut(UART0_BASE, 'u');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, charIn);
    UARTCharPut(UART0_BASE, 10);
    UARTCharPut(UART0_BASE, 13);
}

void GPIOIntHandler(void){
   if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6)&0b01000000)==0b01000000){
       GPIOIntClear(GPIO_PORTD_BASE, GPIO_INT_PIN_6);

       if (modo == 1){
           estado = 1;
       }else if (modo == 2){
           contador_LED++;
           if (contador_LED == 16){
               contador_LED = 0;
           }
       }
   }

   // Verificamos si fue el Pin 4 del puerto E
   if ((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4)&0b00010000)==0b00010000){
           GPIOIntClear(GPIO_PORTE_BASE, GPIO_INT_PIN_4);
           if (modo == 1){
              estado = 1;
          }else if (modo == 2){
              contador_LED--;
              if (contador_LED == -1){
                  contador_LED = 15;
              }
          }
      }
}

void Timer0AIntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    if (modo == 2){
        contador_HEX++;
        estado = 1;
        if (contador_HEX == 0x10){
            contador_HEX = 0;
        }
    }
}

void Timer1AIntHandler(void){
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    secuencia1++;   // Velocidad 1
    secuencia2++;   // Velocidad 2
    // Reiniciar contador para animacion de 7 segmento
    if (secuencia1 == 6){
        secuencia1 = 0;
    }
    if (secuencia2 == 8){
        secuencia2 = 0;
    }
}
