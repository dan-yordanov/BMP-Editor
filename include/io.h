#ifndef IO_H
#define IO_H

#define MAX_FILEPATH_LEN 256 // max filepath length on Windows is 256
#define MAX_INSTRUCTIONS_LEN 16 // up to 16 instructions (arbitrary)
#define IO_ERROR 1

char* get_input_path(int argc, char *argv[], char *input_path);
char* get_output_path(int argc, char *argv[], char *ouput_path);
char* get_instructions(int argc, char *argv[], char *instructions);

#endif