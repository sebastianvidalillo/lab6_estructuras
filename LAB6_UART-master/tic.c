/***************************************************
* Nombre del modulo: tic.c
*
* Modulo creado para la asignatura Elo312
* Laboratorio de Estructura de Computadores
* del departamento de Electrónica de la Universidad
* Técnica Federico Santa María. El uso o copia
* está permitido y se agracede mantener el nombre 
* de los creadores.
*
* Escrito inicialmente el 01/01/2004 Por Michael Kusch & Wolfgang Freund
* Modificado el 26/10/2014           Por Andrés Llico.
*
* Descripción del módulo:
* Modulo para encuestar teclado.
* Contiene las funciones que permiten configurar el
* timer A para encuestar el teclado matricial.
***************************************************/

/*  Include section
*
***************************************************/
#include "tic.h"
#include "keyboard.h"
#include "uart.h"
#include "display.h"
#include <stdio.h>

/*  Defines section
*
***************************************************/

/*  Local Function Prototype Section
*
***************************************************/

/*  Global Variables Section
*
***************************************************/
unsigned int TIC_CLK = 0;                   // Cuenta los tics.
unsigned char BUT_CLK;



////////////////////////////////////////
//       RUTINAS IMPLEMENTADAS        //
//                                    //

/**************************************************
* Nombre    		: void tic_init(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Inicializa el Timer A.
**************************************************/
void tic_init()
{
    TACTL = TASSEL_1 | ID_3 | TAIE;         // ACLK | 1/8 | Interrupt enabled
    TACCR0 = (int)((32768 / TIC_1SEG) / 8) - 1;
    TAR = 0;
    TACTL |= MC_1;                          // UP Mode (count till TACCR0)
}

/**************************************************
* Nombre    		: void tic_tic(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Encuesta el estado de botones
cada 10 [ms].
**************************************************/
void tic_tic()
{
    if (++TIC_CLK == TIC_1SEG)              // intervalo equivale a 1 [s]?
    {
        TIC_CLK = 0;
        // SWclk();                         // incrementa SoftWareClock en 1 segundo
    }

    if (++BUT_CLK == TIC_1SEG / 100)        // comprueba buttons cada 10[ms]
    {
        BUT_CLK = 0;
        keyboard_tec_matrix();
    }
}

/**************************************************
* Nombre    		: __interrupt void tic_timerA1_interrupt(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Vector de interrupciones del
Timer A.
**************************************************/
#pragma vector = TIMERA1_VECTOR 
__interrupt void tic_timerA1_interrupt(void)
{
        if ((P2IN & 0x80) == 0x80){
          uart_0_init_p1(); //9600 baud rate
          }
        else if ((P2IN & 0x80) == 0x00){
          uart_0_init_p2(); //4800 baud rate
        }

    switch (TAIV)
    {
    case 0x0A:    // generado por TAIFG?
        tic_tic();
        break;
    }
}
