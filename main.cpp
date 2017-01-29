#include <stdio.h>
#include <util/delay.h>

int main(void)
{
	while(1) {
		printf("hello, world\n");
		_delay_ms(1000);
	}
	return 0;
}
