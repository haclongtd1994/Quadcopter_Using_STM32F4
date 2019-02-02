#include "delay_using_systick.h"

#define write_reg(addr, value)      *((unsigned long int *)(addr)) = (value)
#define read_reg(addr, mask)        *((unsigned long int *)(addr)) & (mask)

void systick_init(void)
{
	unsigned int temp_reg;
	/* Disable systick */
	write_reg(SYSTICK_CSR, 0x00u);

	/* Source Clock */
	temp_reg = read_reg(SYSTICK_CSR, ~(1 << 2));
	temp_reg |= (1 << 2);
	write_reg(SYSTICK_CSR, temp_reg);

	/* Clear Current value */
	write_reg(SYSTICK_CVR, 0x00u );
}
void start_systick(unsigned int value)
{
	unsigned int temp_reg;

	/* set reload value and current value*/
	write_reg(SYSTICK_RVR, value );
	write_reg(SYSTICK_CVR, 0x00u );

	/* Start timer */
	temp_reg = read_reg(SYSTICK_CSR, ~(1 << 0));
	temp_reg |= 1 << 0;
	write_reg(SYSTICK_CSR, temp_reg);
}
unsigned int get_flag_systick(void)
{
	unsigned int tempreg;

    /* COUNTFLAG */
    tempreg = read_reg(SYSTICK_CSR, 1 << 16);
    tempreg = tempreg >> 16;

    return tempreg;
}
void delay_ms_systick(unsigned int value)
{
	while(value--)
	{
		start_systick(0xBB7F);  /* 1ms */
		while(get_flag_systick() == 0);
	}
}
void delay_us_systick(unsigned int time)
{
  while(time--){
    start_systick(0x2f);//delay 1 us for source clock = 48Mhz - tru cho thoi gian cai dat
    while(get_flag_systick() == 0);
	}
}
