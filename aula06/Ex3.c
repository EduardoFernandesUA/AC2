#include <detpic32.h>

int main(void) {
    TRISBbits.TRISB4 = 1;       // RBx digital output disconnected 
    AD1PCFGbits.PCFG4= 0;       // RBx configured as analog input 
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this 
                                //  mode an internal counter ends sampling and 
                                //  starts conversion 
    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter 
                                //  interrupt is generated. At the same time, 
                                //  hardware clears the ASAM bit 
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns) 
    AD1CON2bits.SMPI = 4-1;    // Interrupt is generated after XX samples 
                                //  (replace XX by the desired number of 
                                //  consecutive samples) 
    AD1CHSbits.CH0SA = 4;       // replace x by the desired input  
                                //  analog channel (0 to 15) 
    AD1CON1bits.ON = 1;         // Enable A/D converter 
                                //  This must the last command of the A/D 
                                //  configuration sequence 

    while(1) {
        AD1CON1bits.ASAM = 1;
        while( IFS1bits.AD1IF == 0 );
        // printf("\r%04d", ADC1BUF0);
        int *p = (int *)(&ADC1BUF0);
        int i;
        printf("\r");
        for (i = 0; i < 16; i++)
        {
            printf("%4d ", p[i*4]);
        }
        int VAL_AD = ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3;
        printf(" V: %d", ((VAL_AD/4)*33+511)/1023);
        
        IFS1bits.AD1IF = 0;
    }

    return 0;
}

550

550 * 33


