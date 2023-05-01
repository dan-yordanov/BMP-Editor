#include <stdio.h>
#include <stdlib.h>
#include "operations.h"
#include "io.h"



void do_instruction(char choice)
{
	switch(choice)
	{
		case 'a':
			//flip_img();
			printf("Изображението бе обърнато хоризонтално / Image was flipped\n\n");
			break;
		case 'b':
			//invert_img();
			printf("Изобрежанието бе инвертирано / Image was inverted\n\n");
			break;
		case 'c':
			//rotate_img();
			printf("Изобрежанието бе завъртяно надясно / Image was rotated to the right\n\n");
			break;
		case 'd':
			printf("Приключване на  четене инструкции / Stopped reading instructions\n\n");
			break;
		default:
			printf("Невалиден избор / Invalid choice\n\n");
			break;
	}
}
