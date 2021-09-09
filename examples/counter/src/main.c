#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dependencies/dcw/dcw.h"

extern void remove_html_with_class(char* class_name);
extern void display_html(html_t raw_html);

html_t main_h =
#include "../frontend/hello_world.cml"
    ;

int counter = 0;

EMSCRIPTEN_KEEPALIVE
void increase_counter()
{
    remove_html_with_class("counter"); 
    counter++;
    display_html(main_h);
}

EMSCRIPTEN_KEEPALIVE
html_t get_counter()
{
    html_t tmp = malloc(128 * sizeof(char));
    sprintf(tmp, "%d\n", counter);
    return tmp;
}

int main()
{
    display_html(main_h);
}