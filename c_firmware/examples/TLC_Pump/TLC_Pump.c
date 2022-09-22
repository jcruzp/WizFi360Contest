/*****************************************************************************
* | File      	:   TLC_Control.c
* | Author      :   Jose A. Cruz P.
* | Function    :
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-08-22
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include "port_common.h"
#include "FreeRTOSConfig.h"
#include <stdio.h>

/**
 * ----------------------------------------------------------------------------------------------------
 * Macros
 * ----------------------------------------------------------------------------------------------------
 */
/* Clock */
#define PLL_SYS_KHZ (133 * 1000)

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */
extern int stdout_init(void);
extern int32_t socket_startup(void);
extern int32_t mqtt_thing(void);

extern void init_ws2812b(void);
extern void put_pixel(uint32_t pixel_grb);
extern uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);

/* Clock */
static void set_clock_khz(void)
{
    // set a system clock frequency in khz
    set_sys_clock_khz(PLL_SYS_KHZ, true);

    // configure the specified clock
    clock_configure(
        clk_peri,
        0,                                                // No glitchless mux
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, // System PLL on AUX mux
        PLL_SYS_KHZ * 1000,                               // Input frequency
        PLL_SYS_KHZ * 1000                                // Output (must be same as no divider)
    );
}

/*-----------------------------------------------------------------------------
 * Application main thread
 *----------------------------------------------------------------------------*/
static void app_main(void *argument)
{
    int32_t status;
    printf("TLC_Pump WiFi init ...\r\n");

    init_ws2812b();

    put_pixel(urgb_u32(0xff, 0, 0)); // Red

    status = socket_startup();
    if (status == 0)
    {
        put_pixel(urgb_u32(0, 0xff, 0)); // Green
        mqtt_thing();
    }
    else {
        printf("TLC_Pump error ...\r\n");
        put_pixel(0); // Turn Off
    }
}

static const osThreadAttr_t app_main_attr = {
    .stack_size = 4096U,
};

// #define WIFI_THREAD_STACK_ARR_SIZE   WIFI_THREAD_STACK_SIZE
// #define THREAD_CC_ATTR     __attribute__((section(".bss.os.threadLCD.cb")))

// /* Stack memory */
// static uint8_t WiFi_ThreadCb[OS_THREAD_CB_SIZE]                __ALIGNED(4) THREAD_CC_ATTR;
// static uint8_t WiFi_ThreadStackArr[WIFI_THREAD_STACK_ARR_SIZE] __ALIGNED(8);

// /* WiFi thread */
// const osThreadAttr_t WiFi_Thread_AttrAA = {
//   .name       = "LCD Thread",
//   .attr_bits  = osThreadDetached,
//   .cb_mem     = WiFi_ThreadCb,
//   .cb_size    = sizeof(WiFi_ThreadCb),
//   .stack_mem  = WiFi_ThreadStackArr,
//   .stack_size = sizeof (WiFi_ThreadStackArr),
//   .priority   = WIFI_THREAD_PRIORITY,
//   .tz_module  = 0,
// };

/*-----------------------------------------------------------------------------
 * Application initialization
 *----------------------------------------------------------------------------*/
void app_initialize(void)
{
    osThreadNew(app_main, NULL, &app_main_attr);
}

// uint8_t ucHeap[configTOTAL_HEAP_SIZE] __attribute__((section(".my_heap")));

int main(void)
{

    set_clock_khz();
    SystemCoreClockUpdate();

    stdio_init_all();

    osKernelInitialize(); // Initialize CMSIS-RTOS
    app_initialize();     // Initialize application
    osKernelStart(); // Start thread execution
    for (;;)
    {
    }
}
