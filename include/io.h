#ifndef IO_H
#define IO_H

#define MAX_FILEPATH_LEN 256 // max filepath length on Windows is 256
#define MAX_INSTRUCTIONS_LEN 16 // up to 16 instructions (arbitrary)
#define IO_ERROR 1

// we'll get needed strings either from cmd args or stdin
int get_input_path(int argc, char *argv[], char *input_path);
int get_output_path(int argc, char *argv[], char *ouput_path);
int get_instructions(int argc, char *argv[], char *instructions);

#endif
