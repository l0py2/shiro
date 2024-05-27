#ifndef INPUT_H
#define INPUT_H

void decode_spaces(char* str);
void extract_parameters(int* limit, int* page);
void extract_input(char* input, char* name, int* rating);
void extract_update_input(char* input, int* id, char* name, int* rating);
void extract_delete_input(char* input, int* id);

#endif
