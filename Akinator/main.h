#include <stdio.h>

const int USER_IS_STUPID = -3802;

void help();

bool is_request_load_from_file(const char* request);

bool is_request_game(const char* request);

bool is_request_make_the_definition(const char* request);

bool is_request_load_to_file(const char* request);

bool is_request_finish_the_program(const char* request);
