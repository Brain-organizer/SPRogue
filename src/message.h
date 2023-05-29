#ifndef MESSAGE_HEADER
#define MESSAGE_HEADER

void init_message();
void add_message(const char *format, ...);
void draw_message();
void free_message();

#endif