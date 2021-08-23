#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include "build/dcw.h"

extern void display_html(html_t raw_html);

EMSCRIPTEN_KEEPALIVE
html_t add_two_numbers(int a, int b)
{
    char *tmp = malloc(128 * sizeof(int));
    int result = a + b;
    sprintf(tmp, "%d", result);
    return tmp;
}

EMSCRIPTEN_KEEPALIVE
html_t hello_world()
{
    return "Hello, World!";
}

int main()
{
    html_t html =
#include "frontend/hello_world.cml"

        display_html(html);
}