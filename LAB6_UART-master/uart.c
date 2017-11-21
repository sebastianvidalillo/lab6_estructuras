/***************************************************
* Nombre del modulo: uart.c
*
* Modulo creado para la asignatura Elo312
* Laboratorio de Estructura de Computadores
* del departamento de Electrónica de la Universidad
* Técnica Federico Santa María. El uso o copia
* está permitido y se agracede mantener el nombre 
* de los creadores.
*
* Escrito inicialmente el 25/11/2013 Por Mauricio Solís
*
* Descripción del módulo:
* Modulo driver UART
* Contiene las funciones que permiten manejar el
* modulo UART.
***************************************************/

/*  Include section
*
***************************************************/
#include "msp430_version.h"// Depende del uC que Ud. esté ocupando.
#include "uart.h"
#include "display.h"
#include <stdio.h>
#include "tic.h"

/*  Defines section
*
***************************************************/
int h=0;
int b=0;
int u=0;
int pregunta=0;
int azul,rojo,verde;
int enviar=0;
/*  Local Function Prototype Section
*
***************************************************/

/*  Global Variables Section
*
***************************************************/


////////////////////////////////////////
//       RUTINAS IMPLEMENTADAS        //
//                                    //

/**************************************************
* Nombre    		: void uart_0_init_p1(void)
* returns			: void
* Creada por		: Mauricio Solís
* Fecha creación	: 25/11/2013
* Descripción		: Inicializa el modulo UART
* a una velocidad de app 9600 baudios
**************************************************/
void uart_0_init_p1()
{
	P3SEL |= 0x30;
	ME1 |= UTXE0 + URXE0;
	if ((P2IN & 0x40) == 0x40){
          U0CTL |= CHAR; // CHAR = 1 , 8 BITS. CHAR = 0, 7 BITS
        }
        else if ((P2IN & 0x40)== 0x00){
          U0CTL &= 0xEF;
        }
	UTCTL0 |= SSEL0;
        if ((P2IN & 0x20) == 0x20){
          U0CTL |= SPB;
        }
        else if ((P2IN & 0x20) == 0x00){
          U0CTL &= 0xDF;
        //  printf("1s");
        }
	//UTCTL0 = 0x10;
	UBR00 = 0x03;
	UBR10 = 0x00;
	UMCTL0 = 0x4A;
	UCTL0 &= ~SWRST;
        IE1 |= URXIE0;
        
}

/**************************************************
* Nombre            : void uart_0_init_p2(void)
* returns           : void
* Creada por        : Mauricio Solís
* Fecha creación    : 25/11/2013
* Descripción       : Inicializa el modulo UART
* a una velocidad de 2400 baudios (con iterrupción
* de recepción).
**************************************************/
void uart_0_init_p2()
{
	P3SEL |= 0x30;
	ME1 |= UTXE0 + URXE0;
	if ((P2IN & 0x40) == 0x40){
          U0CTL |= CHAR; // CHAR = 1 , 8 BITS. CHAR = 0, 7 BITS
        }
        else if ((P2IN & 0x40)== 0x00){
          U0CTL &= 0xEF;
        }
	UTCTL0 |= SSEL0;
        if ((P2IN & 0x20) == 0x20){
          U0CTL |= SPB;
        }
        else if ((P2IN & 0x20) == 0x00){
          U0CTL &= 0xDF;
        }
	UBR00 = 0x06;
	UBR10 = 0x00;
	UMCTL0 = 0xEF;
	UCTL0 &= ~SWRST;
	IE1 |= URXIE0;
}

/**************************************************
* Nombre            : void uart_0_send(char arg1)
* returns           : void
* arg 1             : Caracter a ser enviado
* Creada por        : Mauricio Solís
* Fecha creación    : 25/11/2013
* Descripción       : Envia un caracter por la UART0.
**************************************************/
void uart_0_send(char c)
{
	while (!(IFG1 & UTXIFG0));
	TXBUF0 = c;
}
//                                    //
////////////////////////////////////////


////////////////////////////////////////
//     RUTINAS NO IMPLEMENTADAS       //
//                                    //

/**************************************************
* Nombre    		: __interrupt void uart_uart0_rx(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Vector de interrupciones
del RX del modulo UART.
**************************************************/
#pragma vector=UART0RX_VECTOR
__interrupt void uart_uart0_rx (void)
{
  static int control_slave =0;
  if ((P2IN & 0x10)==0x10){
    control_slave = 1; //control
  }
  else{
    control_slave = 0;
  }
  
  if (control_slave == 0 ){
    if (U0RXBUF != 63){
        if (U0RXBUF != 126){
          if (u==0){ //rojo
            rojo = U0RXBUF;
            u+=1;
            TBCCR1 = (int)(U0RXBUF*12)/100;
            
          }
          else if(u==1){  //verde
            verde = U0RXBUF;
            u+=1;
            TBCCR2 = (int)(U0RXBUF*12)/100;
            
          }
          else if (u==2){ //azul
            azul = U0RXBUF;
            u=0;
            TBCCR3 = (int)(U0RXBUF*12)/100;
           
          }
        }
      }
   else if (RXBUF0 == 63) {
    if(enviar==0){
      uart_0_send(rojo);
      enviar+=1;
    }
    else if (enviar==1){
      uart_0_send(verde);
      enviar+=1;
    }
    else if (enviar==2){
      uart_0_send(azul);
      enviar=0;
    }
  }
   /* else{
      h=0;
        while(h<330){
          display_clear();
        // printf("Animacion");
          display_set_pos(0x40);
         // printf("A: Menu");
          for(b=0;b<11;b++){
            switch (b){
            case 0:
              TBCCR1=0;
              TBCCR2=0;
              TBCCR3=0;
              udelay(1000000000);
              break;
            case 1:
              TBCCR1=12;
              TBCCR2=0;
              TBCCR3=0;
              udelay(1000000000);
              break;
            case 2:
              TBCCR1=0;
              TBCCR2=12;
              TBCCR3=0;
              udelay(1000000000);
            // break;
            case 3:
              TBCCR1=0;
              TBCCR2=0;
              TBCCR3=12;
              udelay(1000000000);
              break;
            case 4:
              TBCCR1=6;
              TBCCR2=6;
              TBCCR3=0;
              udelay(1000000000);
              break;
            case 5:
              TBCCR1=0;
              TBCCR2=10;
              TBCCR3=2;
              udelay(1000000000);
              break;
            case 6:
              TBCCR1=3;
              TBCCR2=0;
              TBCCR3=1;
              udelay(1000000000);
              break;
            case 7:
              TBCCR1=3;
              TBCCR2=5;
              TBCCR3=2;
              udelay(1000000000);
              break;
            case 8:
              TBCCR1=5;
              TBCCR2=8;
              TBCCR3=3;
              udelay(1000000000);
              break;
            case 9:
              TBCCR1=4;
              TBCCR2=1;
              TBCCR3=8;
              udelay(1000000000);
              break;
            case 10:
              TBCCR1=10;
              TBCCR2=4;
              TBCCR3=7;
              udelay(1000000000);
              break;
            }
            h+=1;
          }
        }
        TBCCR1 = 0;
        TBCCR2 = 0;
        TBCCR3 = 0;
    }*/
  }
  else {  
    if (pregunta==0) {
      display_clear();
      printf("Rojo: %d", RXBUF0);
      pregunta+=1;
    }
    
    else if (pregunta==1) {
      printf("Verde: %d", RXBUF0);
      pregunta+=1;
     }
    
    else if (pregunta==2) {
      printf("Azul: %d", RXBUF0);
      pregunta=0;
    
  }
}

/*if (RXBUF0 == 63) {
  if(enviar==0){
    uart_0_send(rojo);
    enviar+=1;
  }
  else if (enviar==1){
    uart_0_send(verde);
    enviar+=1;
  }
  else if (enviar==2){
    uart_0_send(azul);
    enviar=0;
  }
}*/

}