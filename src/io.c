#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "io.h"



int get_input_path(int argc, char *argv[], char *input_path)
{
	// if cmd argument was given use it
	if (argc > 1)
	{
		if (strlen(argv[1]) > MAX_FILEPATH_LEN)
		{
			fprintf(stderr, "Error: input filepath arg length is too big\n");
			return -1;
		}
		strcpy(input_path, argv[1]);
	}
	// else take input
	else
	{
		printf("\nEnter input path: ");
		if (fgets(input_path, MAX_FILEPATH_LEN + 1, stdin) == NULL) 
		{
			perror("Error failed reading input for input path");
			return -1;
		}
		
		// Remove \n if there is one else exit if \n isn't the only thing in the buffer
		if (input_path[strlen(input_path) - 1] == '\n')
		{
			input_path[strlen(input_path) - 1] = '\0';
		}
		else 
		{
			if (getchar() != '\n')
			{
				fprintf(stderr, "Error: input filepath length is too big\n");
				return -1;
			}
		}
	}
	// check if input file has a .bmp extension
	if (strcmp(&input_path[strlen(input_path) - 4], ".bmp") != 0)
	{
		fprintf(stderr, "Error: input file is not bmp\n");
		return -1;
	}
}



int get_output_path(int argc, char *argv[], char *output_path)
{
	//if cmd argument was given use it
	if (argc > 2)
	{
		if (strlen(argv[2]) > MAX_FILEPATH_LEN)
		{
			fprintf(stderr, "Error: output filepath arg length is too big\n");
			return -1;
		}
		strcpy(output_path, argv[2]);
	}
	//else take input
	else
	{
		printf("\nEnter output path: ");
		if (fgets(output_path, MAX_FILEPATH_LEN + 1, stdin) == NULL) 
		{
			perror("Error failed reading input for ouput path");
			return -1;
		}
		
		// Remove \n if there is one else exit if \n isn't the only thing in the buffer 
		if (output_path[strlen(output_path) - 1] == '\n')
		{
			output_path[strlen(output_path) - 1] = '\0';
		}
		else 
		{
			if (getchar() != '\n')
			{
				fprintf(stderr, "Error: ouput filepath length is too big\n");
				return -1;
			}
		}
	}

	// check if output file has a .bmp extension
	if (strcmp(&output_path[strlen(output_path) - 4], ".bmp") != 0)
	{
		fprintf(stderr, "Error: output file is not bmp\n");
		return -1;
	}

	return 0;
}



int get_instructions(int argc, char *argv[], char *instructions)
{
	// if cmd argument was given use it
	if (argc > 3)
	{
		if (strlen(argv[3]) > MAX_INSTRUCTIONS_LEN)
		{
			fprintf(stderr, "Error: too many instructions given\n");
			return -1;
		}
		strcpy(instructions, argv[3]);
	}
	// else take input, we will read instructions one by one
	else
	{
		printf(	"============================================================================================\n"
				"Моля изберете инструкция чрез въвеждане на съответната буква / Enter a choice (a, b, c, d)\n"
				"a. Обръщане на изображението хоризонттално. (FLIP)\n"
				"b. Прилагане на филтър за инверсия на цветовете. (INVERT)\n"
				"c. Завъртане на изображението надясно. (ROTATE)\n"
				"d. Приключване на програмата. (EXIT)\n"
				"Забележка: само първия символ на входа се чете / Only the first symbol of input is read\n"					
				"============================================================================================\n\n");
		
		int choice; // using a second int var which we'll turn to char to avoid an error/undefined behaviour if getchar() returns EOF
		short int count = 0;
		int c;
		
		do
		{
			printf("%d. Въведетe инструкция / Enter instruction: ", count + 1);
			choice = getchar();
			if (choice == '\n' || choice == EOF)
			{
				choice = 'x'; // the program will just proccess it as an invalid command
				instructions[count] = choice;
				instructions[count + 1] = '\0'; // make sure the string is null-terminated
				count++;
				continue;
			} 
			
			choice = tolower(choice);
			
			instructions[count] = choice;
			instructions[count + 1] = '\0'; // make sure the string is null-terminated
			count++;

			// Clean buffer
			while ((c = getchar()) != '\n' && !feof(stdin)); // feof(stdin) returns a non-zero value if eof is set

		} while(choice != 'd' && count != MAX_INSTRUCTIONS_LEN);
	}

	return 0;
}
