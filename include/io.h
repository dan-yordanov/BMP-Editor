#ifndef IO_H
#define IO_H

#define MAX_FILEPATH_LEN 256 // max filepath length on Windows is 256
#define MAX_INSTRUCTIONS_LEN 16 // up to 16 instructions (arbitrary)
#define IO_ERROR 1

//------------------------------------------------------------------------------------------
// DESCRIPTION: Reads the input filepath of the bmp file that will be edited, either from
// cmd args(1st arg) or stdin user input and stores it to input_path string
//
// PARAMETERS: argc, argv, output_path - the string/array where the input filepath of the
// bmp file to be edited will get stored
//
// RETURN VALUE: 0 on success, -1 on failure
//------------------------------------------------------------------------------------------
int get_input_path(int argc, char *argv[], char *input_path);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Reads all the instructions that should be performed on the inputted bmp
// file, either from cmd args(2nd arg) or stdin input and stores it to instructions
//
// PARAMETERS: argc, argv, instructions - the string/array where the given instructions
// will get stored
//
// RETURN VALUE: 0 on success, -1 on failure
//------------------------------------------------------------------------------------------
int get_instructions(int argc, char *argv[], char *instructions);

//------------------------------------------------------------------------------------------
// DESCRIPTION: Reads the output filepath where the edited bmp will get stored at the end
// of the program either from cmd args(3rd arg) or stdin input and stores it to output_path 
//
// PARAMETERS: argc, argv, output_path - the string/array where the output filepath of the
// edited bmp will get stored
//
// RETURN VALUE: 0 on success, -1 on failure
//------------------------------------------------------------------------------------------
int get_output_path(int argc, char *argv[], char *ouput_path);

#endif
