#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
 
/* EXYNOS ADC_V1 registers definitions */
#define ADC_V1_DATAX		0x0C
 
/* Future ADC_V2 registers definitions */
#define ADC_V2_CON1		0x00
#define ADC_V2_CON2		0x04
#define ADC_V2_STAT		0x08
 
/* Bit definitions for ADC_V2 */
#define ADC_V2_CON1_SOFT_RESET	(1u << 2)
 
#define ADC_V2_CON2_OSEL	(1u << 10)
#define ADC_V2_CON2_ESEL	(1u << 9)
#define ADC_V2_CON2_HIGHF	(1u << 8)
#define ADC_V2_CON2_C_TIME(x)	(((x) & 7) << 4)
#define ADC_V2_CON2_ACH_SEL(x)	(((x) & 0xF) << 0)
#define ADC_V2_CON2_ACH_MASK	0xF
 
/* Bit definitions common for ADC_V1 and ADC_V2 */
#define ADC_CON_EN_START        (1u << 0)
#define ADC_DATX_MASK           0xFFF
 
static volatile uint32_t *adc;
static unsigned char channel;
static uint32_t con1, con2;
static int val0;
static int val3;
 
void exynos_read_raw(unsigned char channel);
 
int main(int argc, char **argv)
{
	int fd;
 
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		printf("Unable to open /dev/mem\n");
		return -1;
	}
 
	adc = mmap(0, getpagesize(), PROT_READ | PROT_WRITE,
					MAP_SHARED, fd, 0x12D10000);
	if (adc < 0) {
		printf("mmap failed.\n");
		return -1;
	}
 
	// exynos_adc_hw_init
	con1 = ADC_V2_CON1_SOFT_RESET;
	*(adc + (ADC_V2_CON1 >> 2)) |= con1;
	con2 = ADC_V2_CON2_OSEL | ADC_V2_CON2_ESEL |
		ADC_V2_CON2_HIGHF | ADC_V2_CON2_C_TIME(0);
	*(adc + (ADC_V2_CON2 >> 2)) |= con2;

	//channel = con2 & ADC_V2_CON2_ACH_MASK;
	//printf("Channel: %d", channel);
 
	while (1)
	{
		// End of A/D conversion
		if (*(adc + (ADC_V2_STAT >> 2)) & 4) {
			/* Read value */
			if (channel == 0x0) {
				val0 = *(adc + (ADC_V1_DATAX >> 2)) & ADC_DATX_MASK;
				//printf("ADC Channel 0 : %d\n", val0);
			} else if (channel == 0x3) {
				val3 = *(adc + (ADC_V1_DATAX >> 2)) & ADC_DATX_MASK;
				printf("ADC Channel 3 : %d\n", val3);
			}
 
			if (channel == 0x0) {
				/* start channel 0 conversion */
				exynos_read_raw(3);
			}
			 else if (channel == 0x3) {
				// start channel 3 conversion
				exynos_read_raw(0);
			}
		}
		channel = con2 & ADC_V2_CON2_ACH_MASK;
		printf("Channel: %d", channel);
		//sleep(1);
		usleep(1000*50); //usleep is in microseconds (multiply by 1000 to get milliseconds)
	}
 
	return 0;
}
 
void exynos_read_raw(unsigned char channel)
{
	con2 = *(adc + (ADC_V2_CON2 >> 2));
	con2 &= ~ADC_V2_CON2_ACH_MASK;
	*(adc + (ADC_V2_CON2 >> 2)) &= con2;
	con2 |= ADC_V2_CON2_ACH_SEL(channel);
	*(adc + (ADC_V2_CON2 >> 2)) |= con2;
 
	con1 = *(adc + (ADC_V2_CON1 >> 2));
	*(adc +(ADC_V2_CON1 >> 2)) = con1 | ADC_CON_EN_START;
}
