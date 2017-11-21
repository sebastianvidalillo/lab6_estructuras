#include "msp430_version.h"            // Depende del uC que Ud. esté ocupando.
#include "uart.h"
#include "tic.h"
#include "display.h"
#include "keyboard.h"
#include "osc.h"
#include <stdio.h>

char mensaje[10] = "Hola Mundo";
int i = 0,j = 0;

void main(void) {
	osc_init_xt2();	
	WDTCTL = WDTPW + WDTHOLD;                    // Se dispone de los 3 relojes del sistema
	P2DIR |= 0x00;  
        P4SEL |= 0x1F;
        P4DIR |= 0x1F;
        TBCCR0 = 12;
        TBCTL = TBSSEL_1 + MC_1;
        TBCCTL0 = OUTMOD_7;
        TBCCTL1 = OUTMOD_7;
        TBCCTL2 = OUTMOD_7;
        TBCCTL3 = OUTMOD_7;
        
        display_init();
        display_clear();
        display_set_pos(0x00);
        tic_init();
        keyboard_init();
        U0CTL = BIT3; //Listen
        printf("Animacion: C");
        display_set_pos(0x40);
        printf("Setear LED: D");
   
        
        
        
        // en los bits 4, 5 y 6 de la puerta 5
	                                   // (MCLK, SMCLK y ACLK)
	_EINT();  
        // Habilita las interrupciones
	/*while(1) {
		uart_0_send(mensaje[i++]);
		while (j < 1000) j++;
		j = 0;
		if (i >= 10)
		i = 0;
    }*/
}

