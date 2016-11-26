// Many Thanks to Youngtae Jo! https://groups.google.com/forum/#!topic/beagleboard/0a4tszlq2y06
//Oluwatoni Ogunmade

.origin 0 // offset of the start of the code in PRU memory
.entrypoint START // program entry point, used by debugger only

#include "steering_pru.hp"

#define ADC_SAMPLE_NO 64
#define SAMPLING_RATE 16000 //Sampling rate(1khz)
#define DELAY_MICRO_SECONDS (1000000 / SAMPLING_RATE) //Delay by sampling rate
#define CLOCK 200000000 // PRU is always clocked at 200MHz
#define CLOCKS_PER_LOOP 2 // loop contains two instructions, one clock each
#define DELAYCOUNT DELAY_MICRO_SECONDS * CLOCK / CLOCKS_PER_LOOP / 1000 / 1000 * 3
#define PRU_EVTOUT_0 3
#define PRU0_R31_VEC_VALID 32
#define BOUNDS 80
#define MAX_STEERING (2600 + BOUNDS)
#define MIN_STEERING (1500 - BOUNDS)

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

//Motor movements
.macro CLOCKWISE
    SET r30.t5
    CLR r30.t3
.endm

.macro ANTICLOCKWISE
    CLR r30.t5
    SET r30.t3
.endm

//Read the current setpoint and the end condition
.macro READ_SETPOINT
    MOV r0, 0x00000000
    LBBO r1, r0, 0, 4
    MOV r0, 0x00000004
    LBBO r9, r0, 0, 4
    
    QBEQ END, r9, 0x01//if the exit condition is selected exit the loop
    SUB r6, r1, BOUNDS
    ADD r7, r1, BOUNDS

.endm

//reads the ADC a number of times and averages the values
.macro READADC
  MOV r4, ADC_SAMPLE_NO
  MOV r5, 0
  
READ:
      //Read ADC from FIFO0DATA
      MOV r2, 0x44E0D100 
      LBBO r3, r2, 0, 4
      ADD r5, r5,r3;add up the readings 
      DELAY
        
      SUB r4, r4,1
      QBNE READ, r4, 0
      LSR r5, r5, 6//divide by 8

.endm

// Starting point
START:
    SWITCH_OFF
   //ADC setup
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

    //set the min and max values
    MOV r11, MIN_STEERING
    MOV r12, MAX_STEERING

// Helps prevent the steering from trying to go past the max and min values
PRESTEER:
    SWITCH_OFF

STEER:
    READ_SETPOINT
    READADC
    
    QBEQ STEER, r9, 0x02

    //switch the motors off if the setpoint is outside of the steering range
    QBLT PRESTEER, r11, r6
    QBGT PRESTEER, r12, r7
    
    //decide what motion to execute based on the setpoint and the current measurement
    QBLT LEFT, r6, r5
    QBGT RIGHT, r7, r5
    SWITCH_OFF
    JMP STEER

LEFT:
    CLOCKWISE
    JMP STEER

RIGHT:
    ANTICLOCKWISE
    JMP STEER

END:
    SWITCH_OFF   
    //Send event to host program
    MOV r31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0 
    HALT
