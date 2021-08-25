#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include "dcw_latest/dcw.h"
// #include "../src/dcw.h" -> for when I'm checking for mem leaks

extern void display_html(html_t raw_html);

EMSCRIPTEN_KEEPALIVE
html_t add_two_numbers(int a, int b)
{
    html_t tmp = malloc(128 * sizeof(char));
    int result = a + b;
    sprintf(tmp, "%d", result);
    return tmp;
}

EMSCRIPTEN_KEEPALIVE
html_t hello_world()
{
    html_t tmp = malloc(128 * sizeof(char));
    sprintf(tmp, "%s", "Hello, World!");
    return tmp;
}

int main()
{
    html_t html =
#include "frontend/hello_world.cml"

        display_html(html);
}