#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <emscripten.h>
#include "../dependencies/dcw/dcw.h"

html_t main_h = HTMLIFY("<div id='main_body'></div>");
html_t list_h = HTMLIFY("<p>%s</p>");

extern html_t render_html_loop(html_t html_formatter, char **elements, int num_elements, bool reverse);
extern void insert_html_at_id(html_t raw_html, char *id);
extern void display_html(html_t raw_html);

html_t list_elements()
{
    const int num_months = 12;

    char *months[num_months] = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"};

    html_t some_html = render_html_loop(list_h, months, num_months, false);
    insert_html_at_id(some_html, "main_body");
    free(some_html);
}

int main()
{
    list_elements();
}