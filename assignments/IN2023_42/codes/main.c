
#include "Fw_global_config.h"   // This defines application specific characteristics

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "RtosTask.h"

/* Include the generic headers required for QORC */
#include "eoss3_hal_gpio.h"
#include "eoss3_hal_rtc.h"
#include "eoss3_hal_timer.h"
#include "eoss3_hal_fpga_usbserial.h"
#include "ql_time.h"
#include "s3x_clock_hal.h"
#include "s3x_clock.h"
#include "s3x_pi.h"
#include "dbg_uart.h"

#include "cli.h"

extern const struct cli_cmd_entry my_main_menu[];

const char *SOFTWARE_VERSION_STR;

extern void qf_hardwareSetup();
static void nvic_init(void);

#define GPIO_OUTPUT_MODE (1)
#define GPIO_INPUT_MODE (0)
void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode);
int PyHal_GPIO_GetDir(uint8_t gpionum);
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval);
int PyHal_GPIO_Get(uint8_t gpionum);

void update_shift_registers(uint8_t *A, uint8_t *B);
void display_on_seven_segment(uint8_t value);

int main(void)
{
    uint8_t A[4] = {1, 1, 0, 1}; // Initial content of shift register A
    uint8_t B[4] = {1, 0, 1, 0}; // Initial content of shift register B
    
    SOFTWARE_VERSION_STR = "qorc-onion-apps/qf_hello-fpga-gpio-ctlr";
    
    qf_hardwareSetup();
    nvic_init();

    dbg_str("\n\n");
    dbg_str("##########################\n");
    dbg_str("Quicklogic QuickFeather FPGA GPIO CONTROLLER EXAMPLE\n");
    dbg_str("SW Version: ");
    dbg_str(SOFTWARE_VERSION_STR);
    dbg_str("\n");
    dbg_str(__DATE__ " " __TIME__ "\n");
    dbg_str("##########################\n\n");

    dbg_str("\n\nHello GPIO!!\n\n");	// <<<<<<<<<<<<<<<<<<<<<  Change me!

    CLI_start_task(my_main_menu);
    HAL_Delay_Init();
    
    // Seven segment display pins
    PyHal_GPIO_SetDir(18, 1);
    PyHal_GPIO_SetDir(21, 1);
    PyHal_GPIO_SetDir(22, 1);
    PyHal_GPIO_SetDir(23, 1);
    PyHal_GPIO_SetDir(24, 1);
    PyHal_GPIO_SetDir(25, 1);
    PyHal_GPIO_SetDir(26, 1);

    for (int i = 0; i < 4; i++) {
        update_shift_registers(A, B);
        HAL_DelayUSec(1000000); // Wait for 1 second between each shift
    }
     if (A[0] == 1 && A[1] == 1 && A[2] == 0 && A[3] == 1 &&
        B[0] == 1 && B[1] == 1 && B[2] == 0 && B[3] == 1) {
        display_on_seven_segment(1); // Display '1'
    } else if (A[0] == 1 && A[1] == 1 && A[2] == 1 && A[3] == 0 &&
               B[0] == 1 && B[1] == 0 && B[2] == 0 && B[3] == 1) {
        display_on_seven_segment(2); // Display '2'
    } else if (A[0] == 0 && A[1] == 1 && A[2] == 0 && A[3] == 1 &&
               B[0] == 1 && B[1] == 1 && B[2] == 0 && B[3] == 1) {
        display_on_seven_segment(3); // Display '3'
    } else if (A[0] == 1 && A[1] == 0 && A[2] == 1 && A[3] == 0 &&
               B[0] == 1 && B[1] == 1 && B[2] == 1 && B[3] == 1) {
        display_on_seven_segment(4); // Display '4'
    } else {
        display_on_seven_segment(0); // Display default, e.g., '0'
    

    while (1);
    }
}

static void nvic_init(void)
{
    // To initialize system, this interrupt should be triggered at main.
    // So, we will set its priority just before calling vTaskStartScheduler(), not the time of enabling each irq.
    NVIC_SetPriority(Ffe0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SpiMs_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CfgDma_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Uart_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FbMsg_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
}

//needed for startup_EOSS3b.s asm file
void SystemInit(void)
{

}

//gpionum --> 0 --> 31 corresponding to the IO PADs
//gpioval --> 0 or 1
#define FGPIO_DIRECTION_REG (0x40024008)
#define FGPIO_OUTPUT_REG (0x40024004)
#define FGPIO_INPUT_REG (0x40024000)
//Set GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
//Before Set/Get GPIO value, the direction must be correctly set
void PyHal_GPIO_SetDir(uint8_t gpionum, uint8_t iomode)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);
    if (iomode) printf("Updated A: %d%d%d%d, B: %d%d%d%d\n", A[0], A[1], A[2], A[3], B[0], B[1], B[2], B[3]);
}

        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    else
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 & (~(0x1 << gpionum));

}

//Get current GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
int PyHal_GPIO_GetDir(uint8_t gpionum)
{
    uint32_t tempscratch32;
    int result = 0;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    result = ((tempscratch32 & (0x1 << gpionum)) ? GPIO_OUTPUT_MODE : GPIO_INPUT_MODE);

    return result;
}

//Set GPIO(=gpionum) to 0 or 1 (= gpioval)
//The direction must be set as Output for this GPIO already
//Return value = 0, success OR -1 if error.
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    //Setting Direction moved out as separate API, we will only check
    //*(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    if (!(tempscratch32 & (0x1 << gpionum)))
    {
        //Direction not Set to Output
        return -1;
    }
    
    tempscratch32 = *(uint32_t*)(FGPIO_OUTPUT_REG);

    if(gpioval > 0)
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 | (0x1 << gpionum);
    }
    else
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 & ~(0x1 << gpionum);
    }    

    return 0;
}

//Get GPIO(=gpionum): 0 or 1 returned (or in errors -1)
//The direction must be set as Input for this GPIO already
int PyHal_GPIO_Get(uint8_t gpionum)
{
    uint32_t tempscratch32;
    uint32_t gpioval_input;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_INPUT_REG);
    gpioval_input = (tempscratch32 >> gpionum) & 0x1;

    return ((int)gpioval_input);
}

void update_shift_registers(uint8_t *A, uint8_t *B)
{
    uint8_t new_bit_A = ((!A[1]) ^ A[3]);
 B[3] = B[2];
 B[2] = B[1];
 B[1] = B[0];
 B[0] = A[3];
 A[3] = A[2]; 
 A[2] = A[1];
 A[1] = A[0];
 A[0] = new_bit_A;
    printf("Updated A: %d%d%d%d, B: %d%d%d%d\n", A[0], A[1], A[2], A[3], B[0], B[1], B[2], B[3]);
}


void display_on_seven_segment(uint8_t value)
{
    uint8_t segments[10] = {
        0b01000000, // 0
        0b01111001, // 1
        0b00100100, // 2
        0b00110000, // 3
        0b00011001, // 4
    };

    uint8_t seg = segments[value];
    PyHal_GPIO_Set(18, (seg >> 0) & 1);
    PyHal_GPIO_Set(21, (seg >> 1) & 1);
    PyHal_GPIO_Set(22, (seg >> 2) & 1);
    PyHal_GPIO_Set(23, (seg >> 3) & 1);
    PyHal_GPIO_Set(24, (seg >> 4) & 1);
    PyHal_GPIO_Set(25, (seg >> 5) & 1);
    PyHal_GPIO_Set(26, (seg >> 6) & 1);
}
