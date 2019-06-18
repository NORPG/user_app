#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "patten.h"
#include <time.h>

int main(int argc, char* argv[])
{
	
	int my_gpio_fd;
	int value;
	uint32_t *pi_di = (uint32_t *) data_in;
	uint32_t *pi_do = (uint32_t *) data_out;

	/*	open mygpio device	*/
	my_gpio_fd = open("/dev/mygpio", O_RDWR);
	
	/*	check return value	*/
	if(my_gpio_fd < 0) {
		printf("[ERROR] can't open mygpio\n\r");
		return -1;
	};
	
	/*
	 *	user application code
	 */
	{
		int index;
		int value;
		clock_t start, end;
		
		start = clock();

		value = (uint32_t) 1 << 1;
		write(my_gpio_fd, &value, (size_t) 0);
		for(index = 0 ; index < 16; index++)
		{
			write(my_gpio_fd, pi_di, index+16);
			pi_di++;
		}

		value = ((uint32_t) 1 << 1) | ((uint32_t) 1 << 0);
		write(my_gpio_fd, &value, (size_t) 0);
		
		value = 0;
		while(value == 0)
			read(my_gpio_fd, &value, (size_t) 1);
		
		for(index = 0 ; index < 4; index++)
		{
			read(my_gpio_fd, pi_do, index+4);
			pi_do++;
		};
		
		value = ((uint32_t) 1 << 1);
		write(my_gpio_fd, &value, 0);
		
		end = clock();
				
		int err = 0;
		for(index = 0 ; index < 16; index++)
		{	
			if(data_out[index] != golden[index])
			{
				printf("%d data is wrong , result = %d, expect = %d\n\r", index, data_out[index], golden[index]);	
				err++;
			}
		}
		printf("err count = %d\n\r", err);

		printf("total excution time is %lf\n\r", (end-start)/(double) CLOCKS_PER_SEC);
	}

	
	/*	close mygpio device	*/
	close(my_gpio_fd);	
}
