// Developed by Youngtae Jo in Kangwon National University (April-2014)

// This program collects ADC from AIN0 with certain sampling rate.
// The collected data are stored into PRU shared memory(buffer) first.
// The host program(ADCCollector.c) will read the stored ADC data
// This program uses double buffering technique. 
// The host program can recognize the buffer status by buffer status variable
// 0 means empty, 1 means first buffer is ready, 2 means second buffer is ready.
// When each buffer is ready, host program read ADC data from the buffer.


.origin 0 // offset of the start of the code in PRU memory
.entrypoint START // program entry point, used by debugger only

#include "steering_pru.hp"

#define ADC_SAMPLE_NO 8
#define SAMPLING_RATE 1000 //Sampling rate(1khz)
#define DELAY_MICRO_SECONDS (1000000 / SAMPLING_RATE) //Delay by sampling rate
#define CLOCK 200000000 // PRU is always clocked at 200MHz
#define CLOCKS_PER_LOOP 2 // loop contains two instructions, one clock each
#define DELAYCOUNT DELAY_MICRO_SECONDS * CLOCK / CLOCKS_PER_LOOP / 1000 / 1000 * 3

.macro DELAY
    MOV r10, DELAYCOUNT
    DELAY:
        SUB r10, r10, 1
        QBNE DELAY, r10, 0
.endm

//switch off the steering control pins
.macro SWITCH_OFF 
    CLR r30.t5    
    CLR r30.t3    
.endm

.macro CLOCKWISE
    SET r30.t5
    CLR r30.t3
.endm

.macro ANTICLOCKWISE
    CLR r30.t5
    SET r30.t3
.endm

.macro READ_SETPOINT
    LBCO r0, CONST_PRUSHAREDRAM, 0, 4 ;first two bytes provide the steering setpoint
//    LBB0 r1, CONST_PRUSHAREDRAM, 2, 2 ;second two bytes provide the mode
    SUB r6, r0, 5
    ADD r7, r0, 5

.endm


.macro READADC ;reads the ADC a number of times and averages the values
  MOV r4, ADC_SAMPLE_NO
  MOV r5, 0
  
READ:
      //Read ADC from FIFO0DATA
      MOV r2, 0x44E0D100 
      LBBO r3, r2, 0, 4
      ADD r5, r5,r3;add up the readings 
      DELAY
        
      SUB r4, r4,1
      QBEQ READ, r4, 0
      LSR r5, r5, 3
.endm

// Starting point
START:
    // Enable OCP master port
    LBCO r0, CONST_PRUCFG, 4, 4
    CLR r0, r0, 4
    SBCO r0, CONST_PRUCFG, 4, 4

    //C28 will point to 0x00012000 (PRU shared RAM)
    MOV r0, 0x00000120
    MOV r1, CTPPR_0
    ST32 r0, r1

    //Init ADC CTRL register
    MOV r2, 0x44E0D040
    MOV r3, 0x00000005
    SBBO r3, r2, 0, 4

    //Enable ADC STEPCONFIG 1
    MOV r2, 0x44E0D054
    MOV r3, 0x00000002
    SBBO r3, r2, 0, 4

    //Init ADC STEPCONFIG 1
    MOV r2, 0x44E0D064
    MOV r3, 0x00000001 //continuous mode
    SBBO r3, r2, 0, 4
    
    READ_SETPOINT
    READADC

    QBGT LEFT, r7,r5
    QBLT RIGHT,r6,r5

LEFT:
    CLOCKWISE
    JMP END
RIGHT:
    ANTICLOCKWISE
    JMP END
END:
    ;SWITCH_OFF   
    //Send event to host program
    MOV r31.b0, PRU0_ARM_INTERRUPT+16 
    HALT

