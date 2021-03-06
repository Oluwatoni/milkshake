/*include Files                                                               *
Many Thanks to Youngtae Jo! https://groups.google.com/forum/#!topic/beagleboard/0a4tszlq2y0
Oluwatoni Ogunmade
******************************************************************************/
// Standard header files
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Driver header file
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

/******************************************************************************
* Local Macro Declarations                                                    * 
******************************************************************************/
#define PRU_NUM 	 0
#define OFFSET_SHAREDRAM 2048		//equivalent with 0x00002000

#define PRUSS0_SHARED_DATARAM    4
#define SAMPLING_RATE 1000 //1khz
#define BUFF_LENGTH SAMPLING_RATE
#define PRU_SHARED_BUFF_SIZE 500
#define CNT_ONE_SEC SAMPLING_RATE / PRU_SHARED_BUFF_SIZE

/******************************************************************************
* Functions declarations                                                      * 
******************************************************************************/
static void Enable_PRU();
static void Enable_ADC();
static unsigned int ProcessingADC1(unsigned int value);

/******************************************************************************
* Global variable Declarations                                                * 
******************************************************************************/
static void *sharedMem;
static unsigned int *sharedMem_int;
const char SLOTS[] = "/sys/devices/platform/bone_capemgr/slots";

/******************************************************************************
* Main                                                                        * 
******************************************************************************/
int main (int argc, char* argv[])
{
  unsigned int ret;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  int i = 0, cnt = 0, total_cnt = 0;
  int target_buff = 1;
  unsigned int sampling_period = 0;

  if(argc != 2){
    printf("\tERROR: Sampling period is required by second\n");
    printf("\t       %s [sampling period]\n", argv[0]);
    return 0;
  }

  sampling_period = atoi(argv[1]);

  /* Enable ADC */
  Enable_ADC();
  /* Enable PRU */
  Enable_PRU();

  /* Initializing PRU */
  prussdrv_init();
//  prussdrv_pru_reset ( 0 );
  ret = prussdrv_open(PRU_EVTOUT_0);
  if (ret){
      printf("\tERROR: prussdrv_open open failed\n");
      return (ret); 
  }
  prussdrv_pruintc_init(&pruss_intc_initdata);
  printf("\tINFO: Initializing.\r\n");
  	
  /* Executing PRU. */
  printf("Writing to ADC\n");
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &sampling_period, 4);
  prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &sharedMem);
  sharedMem_int = (unsigned int* )sharedMem;
  *(sharedMem_int+1) = 2;
  prussdrv_exec_program (PRU_NUM, "./steering_pru.bin");
  usleep(1000000);
  *(sharedMem_int+1) = 1;
  printf("\tINFO: PRU completed transfer.\r\n");
/*  prussdrv_pru_wait_event(PRU_EVTOUT_0);  
  prussdrv_pru_disable(PRU_NUM);
  prussdrv_exit();
  
  prussdrv_init();
  ret = prussdrv_open(PRU_EVTOUT_0);
  if (ret){
      printf("\tERROR: prussdrv_open open failed\n");
      return (ret); 
  }
  prussdrv_pruintc_init(&pruss_intc_initdata);
  printf("\tINFO: Initializing.\r\n");
  	
  // Executing PRU. 
  printf("Writing to ADC\n");
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &sampling_period, 4);
  prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &sharedMem);
  sharedMem_int = (unsigned int* )sharedMem;*/
  *(sharedMem_int+1) = 0;
  //prussdrv_exec_program (PRU_NUM, "./steering_pru.bin");
  usleep(10000000);
  *(sharedMem_int+1) = 1;
  printf("\tINFO: PRU completed transfer.\r\n");
  usleep(10000);
  prussdrv_pru_wait_event(PRU_EVTOUT_0);  

  printf("\n%d\n",*(sharedMem_int+1));
  /* Disable PRU*/
  prussdrv_pru_disable(PRU_NUM);
  prussdrv_exit();
  return(0);
}

/*****************************************************************************
* Local Function Definitions                                                 *
*****************************************************************************/
/* Enable ADC */
static void Enable_ADC()
{
	FILE *ain;

	ain = fopen(SLOTS, "w");
	if(!ain){
		printf("\tERROR: $SLOTS open failed\n");
		return -1;
	}
	fseek(ain, 0, SEEK_SET);
	fprintf(ain, "BB-ADC");
	fflush(ain);
}

/* Enable PRU */
static void Enable_PRU()
{
		FILE *ain;

		ain = fopen(SLOTS, "w");
		if(!ain){
			printf("\tERROR: $SLOTS open failed\n");
			return -1;
		}
		fseek(ain, 0, SEEK_SET);
		fprintf(ain, "PRU-STEERING");
		fflush(ain);
}

/* 
 * FIFO0DATA register includes both ADC and channelID
 * so we need to remove the channelID
 */
static unsigned int ProcessingADC1(unsigned int value)
{
	unsigned int result = 0;

	result = value << 20;
	result = result >> 20;

	return result;
}
