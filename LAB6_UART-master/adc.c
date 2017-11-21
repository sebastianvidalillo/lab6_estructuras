/***************************************************
* Nombre del modulo: adc.c
*
* Modulo creado para la asignatura Elo312
* Laboratorio de Estructura de Computadores
* del departamento de Electr�nica de la Universidad
* T�cnica Federico Santa Mar�a. El uso o copia
* est� permitido y se agracede mantener el nombre 
* de los creadores.
*
* Escrito inicialmente el 01/01/2004 Por Michael Kusch & Wolfgang Freund
* Modificado el 24/09/2014           Por Andr�s Llico.
* Modificado el 31/10/2017           Por Mauricio Sol�s
*
* Descripci�n del m�dulo:
* Modulo driver adc
* Contiene las funciones que permiten manejar el ADC
***************************************************/

/*  Include section
*
***************************************************/
#include "adc.h"

/*  Defines section
*
***************************************************/

/*  Local Function Prototype Section
*
***************************************************/

/*  Global Variables Section*/
float resultado;
int cnt = 0;
float arreglo[5];
float promedio;
float x;

////////////////////////////////////////
//       RUTINAS IMPLEMENTADAS        //
//                                    //

/**************************************************
* Nombre    		: int adc_single_channel_single_conv(int arg1)
* returns			: Resultado de la conversi�n
* arg1				: Canal a convertir
* Creada por		: Michael Kusch
* Fecha creaci�n	: 01/01/2004
* Descripci�n		: Realiza una conversi�n simple
de un solo canal.
**************************************************/
int adc_single_channel_single_conv(int ch)
{
	ADC12CTL0 &= ~ENC;
	ADC12MCTL0 &= 0xF0; 		// Se limpia canal anterior
	ADC12MCTL0 |= (ch & 0x0F);  // Selecciona canal a convertir
	ADC12CTL0 |= ENC;           // habilita conversi�n
	ADC12CTL0 |= ADC12SC;
	while (ADC12CTL0 & ADC12SC);
	
	return ADC12MEM0;    		// Los resultados de la conversi�n
}

/**************************************************
* Nombre    		: void adc_init_single_channel_single_conv(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creaci�n	: 01/01/2004
* Descripci�n		: Inicializa el ADC en modo de
conversi�n simple de un solo canal.
**************************************************/
void adc_init_single_channel_single_conv()
{
    P6SEL |= 0xFF;
    P6DIR &= 0x00;
    ADC12CTL0 &= ~ENC;
    ADC12CTL0 = SHT1_15 | SHT0_15 | ADC12ON;
    ADC12CTL1 = CSTARTADD_0 | SHS_0 | SHP | ADC12DIV_7 | ADC12SSEL_3 | CONSEQ_0;
    //CONSEQx = 1 => MODO DEL ADC12: Secuencia de canales.
    ADC12MCTL0 = SREF_0;
    ADC12IE = 0x00; // deshabilitar interrupciones
}
//                                    //
////////////////////////////////////////


////////////////////////////////////////
//     RUTINAS NO IMPLEMENTADAS       //
//                                    //

/**************************************************
* Nombre    		: void adc_init_single_channel_single_conv_it(void)
* returns			: void
* Creada por		: Mauricio Sol�s
* Fecha creaci�n	: 31/10/2017
* Descripci�n		: Inicializa el ADC en modo de
* conversi�n simple de un solo canal y habilita la interrupci�n
* de t�rmino de conversi�n.
**************************************************/
void adc_init_single_channel_single_conv_it()
{
    P6SEL |= 0xFF;
    P6DIR &= 0x00;
    ADC12CTL0 &= ~ENC; // habilita conversion
    ADC12CTL0 = SHT1_15 | SHT0_15 | ADC12ON; // 15 ciclos de reloj dura el muestreo de los canales | enciende el conversor
    ADC12CTL1 = CSTARTADD_0 | SHS_0 | SHP | ADC12DIV_7 | ADC12SSEL_3 | CONSEQ_0; // registro de memoria | fuente de reloj que inicia la conversion (ADC12SC) | modo de captura (pulso) | dividir en 8 el reloj | reloj | unico canal (modo)
    //CONSEQx = 1 => MODO DEL ADC12: Secuencia de canales.      
    ADC12MCTL0 = SREF_0;
    ADC12IE = 0x01; // habilitar interrupciones

}

/**************************************************
* Nombre    		: int adc_single_channel_single_conv_it(int arg1)
* returns			: void
* arg1				: Canal a convertir
* Creada por		: Mauricio Sol�s
* Fecha creaci�n	: 31/10/2017
* Descripci�n		: Inicia la conversi�n de un canal, cuyo resultado
* ser� capturado en la rutina de interrupci�n.
**************************************************/
void adc_single_channel_single_conv_it(int ch)
{
        ADC12CTL0 &= ~ENC;
	ADC12MCTL0 &= 0xF0; 		// Se limpia canal anterior
	ADC12MCTL0 |= (ch & 0x0F);  // Selecciona canal a convertir
	ADC12CTL0 |= ENC;           // habilita conversi�n
	ADC12CTL0 |= ADC12SC;
}



/**************************************************
* Nombre    		: void adc_init_seq_channel_single_conv(void)
* returns			: void
* Creada por		: Andr�s Llico
* Fecha creaci�n	: 24/09/2014
* Descripci�n		: Inicializa el ADC en modo de
conversi�n simple de una secuencia de canales.
**************************************************/
void adc_init_seq_channel_single_conv()
{
    //fill in

}

/**************************************************
* Nombre    		: void adc_seq_channel_single_conv(int *arg1, int *arg2)
* returns			: void
* arg1				: Arreglo con resultados de la conversi�n
* arg2				: Arreglo de canal a convertir
* Creada por		: Andr�s Llico
* Fecha creaci�n	: 24/09/2014
* Descripci�n		: Realiza una conversi�n simple
de una secuencia de canales.
**************************************************/
void adc_seq_channel_single_conv(int *results, int *ch)
{
    //fill in

}

/**************************************************
* Nombre    		: float _resultado(void)
* returns			: int
* Creada por		: Papi Busta
* Fecha creaci�n	: 10/11/2017
* Descripci�n		: Entrega el promedio de la 
conversion
**************************************************/
float _resultado(void)
{
  return promedio;
}  

/**************************************************
* Nombre    		: __interrupt void adc_ADC12_interrupt(void)
* returns			: void
* Creada por		: Mauricio Solis
* Fecha creaci�n	: 01/01/2013
* Descripci�n		: Vector de interrupciones del
ADC.
**************************************************/
#pragma vector = ADC12_VECTOR
__interrupt void adc_ADC12_interrupt(void)
{


}
//                                    //
////////////////////////////////////////

