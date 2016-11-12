#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <sstream>

#define PRU_NUM 	 0
#define OFFSET_SHAREDRAM 2048		//equivalent with 0x00002000
#define PRUSS0_SHARED_DATARAM    4
#define STEERING_NEUTRAL 2048

//function declaration
static void Enable_PRU();
static void Enable_ADC();
static void Enable_PWM();
static unsigned int ProcessingADC1(unsigned int value);

//global variable declarations
static void *sharedMem;
static unsigned int *sharedMem_int;
const char SLOTS[] = "/sys/devices/bone_capemgr.9/slots";

int main(int argc, char **argv)
{
  unsigned int ret;
  unsigned int steering_neutral = STEERING_NEUTRAL;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

  Enable_PRU();
  Enable_ADC();
  Enable_PWM();

  ros::init(argc, argv, "vehicle_control");
  ros::NodeHandle n("~");
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  ros::Rate loop_rate(10);

 /* Initializing PRU */
  prussdrv_init();
  ret = prussdrv_open(PRU_EVTOUT_0);
  if (ret){
      printf("\tERROR: prussdrv_open open failed\n");
      return (ret); 
  }
  prussdrv_pruintc_init(&pruss_intc_initdata);
  printf("\tINFO: Initializing.\r\n");
  	
  /*runs the code for a short period of time with the steering motor disabled
   to read from the ADC and clear the noise*/
  printf("Writing to ADC\n");
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &steering_neutral, 4);
  prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &sharedMem);
  sharedMem_int = (unsigned int* )sharedMem;
  *(sharedMem_int+1) = 2;
  prussdrv_exec_program (PRU_NUM, "./steering_pru.bin");
  usleep(100000);
  *(sharedMem_int+1) = 1;
  printf("\tINFO: PRU completed initial test.\r\n");
  prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

  *(sharedMem_int+1) = 0;
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &steering_neutral, 4);
  prussdrv_exec_program (PRU_NUM, "./steering_pru.bin");

  while (ros::ok()){
    ros::spinOnce();
    loop_rate.sleep();
  }
  //tells the PRU to halt
  *(sharedMem_int+1) = 1;
  printf("\tINFO: PRU disabled.\r\n");
  prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);  
  printf("\n%d\n",*(sharedMem_int+1));
  /* Disable PRU*/
  prussdrv_pru_disable(PRU_NUM);
  prussdrv_exit();

  return 0;
}

/* Enable ADC */
static void Enable_ADC(){
  FILE *ain;
  ain = fopen(SLOTS, "w");
  if(!ain){
    printf("\tERROR: $SLOTS open failed\n");
    return;
  }
  fseek(ain, 0, SEEK_SET);
  fprintf(ain, "cape-bone-iio");
  fflush(ain);
}

/* Enable PRU */
static void Enable_PRU(){
  FILE *ain;
  ain = fopen(SLOTS, "w");
  if(!ain){
    printf("\tERROR: $SLOTS open failed\n");
    return;
  }
  fseek(ain, 0, SEEK_SET);
  fprintf(ain, "PRU-STEERING");
  fflush(ain);
}

/* Enable PRU */
static void Enable_PWM(){
  FILE *ain;
  ain = fopen(SLOTS, "w");
  if(!ain){
    printf("\tERROR: $SLOTS open failed\n");
    return;
  }
  fseek(ain, 0, SEEK_SET);
  fprintf(ain, "am33xx_pwm");
  fprintf(ain, "bone_pwm_P9_28");
  fflush(ain);
}
