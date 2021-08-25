/** \file 
    A framework to write HTML directly within C and have it call C functions. 
    Users interact with it through the \c display function, and the \c html_t and \c HTML macros in dcw.h.
*/

/*
 * Copyright 2021 Danilo Chiarlone
 * LICENSE: GNU AFFERO GENERAL PUBLIC LICENSE
*/

#include <stdio.h>      // needed for sprintf
#include <stdlib.h>     // needed for free
#include <string.h>     // needed for strcat, and strlen
#include <regex.h>      // needed for regmatch_t, regex_t, regcomp, regexec, and REG_NOMATCH
#include <emscripten.h> // needed for EMSCRIPTEN_KEEPALIVE

#include "dcw.h" // needed for html_t, and HTML

/** \mainpage
    \n
    To view the source code, refer to: https://github.com/danbugs/dancing_web \n
    For instructions on using DCW in your own project, refer to: https://github.com/danbugs/dancing_web#readme \n
*/

/** A JS funtion that renders the HTML to the DOM.
    \param html The HTML you want to render. This should be of type html_t or char*.
*/
extern void displayInner(html_t html); // this comes from dcw.js

/** Find and execute any C function calls within the raw html.
    \param raw_html The HTML you want to parse. This should be of type html_t or char*.
    \return The HTML that is ready to be rendered.
*/
html_t parse_html(html_t raw_html)
{
    regmatch_t M[1];

    regex_t regex;
    int re;
    re = regcomp(&regex, "\\${\\w*\\([^$]*\\)}\\$", 0);
    re = regexec(&regex, raw_html, 1, M, 0);

    if (!re)
    {
        int precision = (M[0].rm_eo - 4) - (M[0].rm_so);
        char to_eval[precision + 2];
        sprintf(to_eval, "_%.*s", precision, raw_html + M[0].rm_so + 2);
        char * evaled = (char *) emscripten_run_script_int(to_eval);

        char rendered_html[strlen(raw_html) - (M[0].rm_eo - M[0].rm_so) + strlen(evaled) + 1];
        char html_until_marker[M[0].rm_so + 1];
        char html_after_marker[strlen(raw_html) - M[0].rm_eo + 1];

        sprintf(html_until_marker, "%.*s", (int)M[0].rm_so, raw_html);
        sprintf(html_after_marker, "%s", &raw_html[M[0].rm_eo]);

        strcat(rendered_html, html_until_marker);
        strcat(rendered_html, evaled);
        strcat(rendered_html, html_after_marker);

        free((void *) evaled);
        return parse_html(rendered_html);
    }
    else if (re == REG_NOMATCH)
    {
        return raw_html;
    }
    else
    {
        html_t error_html =
#include "error.cml"

            return error_html;
    }
}

/** A wrapper for \c parse_html, and the JS funtion \c displayInner .

    \param raw_html The HTML you want to display. This should be of type html_t or char*.
*/
EMSCRIPTEN_KEEPALIVE
void display_html(html_t raw_html)
{
    html_t rendered_html = parse_html(raw_html);
    displayInner(rendered_html);
}
