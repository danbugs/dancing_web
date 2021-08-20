#include <emscripten.h>
#include "../src/dancing_web.h"

EMSCRIPTEN_KEEPALIVE
int add_two_numbers(int a, int b)
{
    return a + b;
}

int main()
{
    html_t html =
        #include "public/hello_world.cml"

    display(html);
}