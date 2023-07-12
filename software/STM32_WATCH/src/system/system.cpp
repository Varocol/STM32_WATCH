#include "system.h"

/**
 * @brief       系统初始化
 * @retval      None
 */
void system_init()
{
    clock_init();
    power_on();
    nvic_group_init();
    uart_init();
    led_init();
    keys_init();
    keys_intterupt_init();
    beep_init();
    beep_interrupt_init();
}

#ifdef __cplusplus
extern "C"
{
#endif
    __ASM(".global __use_no_semihosting");
    /* 定义 _sys_exit() 以避免使用半主机模式 */
    void _sys_exit(int x)
    {
        x = x;
    }
    void _ttywrch(int ch)
    {
        ch = ch;
    }

    /* 标准库需要的支持类型 */
    struct __FILE
    {
        int handle;
    };
    FILE __stdout;

    // 重定向c库函数printf到串口,重定向后可使用printf函数,前提是得开启DEBUG_USARTx
    int fputc(int ch, FILE *f)
    {
        /* 发送一个字节数据到串口 */
        uart_send_data((uint8_t)ch);
        return ch;
    }
    // 重定向c库函数scanf到串口,重定向后可使用scanf、getchar等函数,前提是得开启DEBUG_USARTx,要关闭RXNE中断才可以正常使用
    int fgetc(FILE *f)
    {
        /* 等待串口输入数据 */
        return (int)uart_receive_data;
    }
#ifdef __cplusplus
}
#endif