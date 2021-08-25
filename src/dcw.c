/** \file
    A framework to write HTML directly within C and have it call C functions.
    Users interact with it through the \c display function, and the \c html_t
   and \c HTML macros in dcw.h.
*/

/*
 * Copyright 2021 Danilo Chiarlone
 * LICENSE: GNU AFFERO GENERAL PUBLIC LICENSE
 */

#include "dcw.h"
#include "dcw_internal.h"

#include <emscripten.h> // needed for EMSCRIPTEN_KEEPALIVE
#include <regex.h>      // needed for regmatch_t, regex_t, regcomp, regexec, and REG_NOMATCH
#include <stdio.h>      // needed for sprintf
#include <stdlib.h>     // needed for free
#include <string.h>     // needed for strcat, and strlen

/** \mainpage
    \n
    To view the source code, refer to: https://github.com/danbugs/dancing_web \n
    For instructions on using DCW in your own project, refer to:
   https://github.com/danbugs/dancing_web#readme \n
*/

/** The HTML that is displayed to the users then there is DCW encounters an error.
*/
html_t ERROR_HTML =
#include "error.cml"
    ;

/** A JS funtion that renders the HTML to the DOM.
    \param html The HTML you want to render. This should be of type html_t or char*.
*/
extern void displayInner(html_t html);

/** A wrapper around \c parse_html_core to allow setting default argument
   values. \param in This should contain the some raw html and, maybe, the type
   of marker to look for. \return The HTML that is ready to be rendered.
*/
html_t parse_html_wrapper(parse_html_args in)
{
    html_t raw_html_out =
        in.raw_html ? in.raw_html : ERROR_HTML;

    enum marker_type marker_mid = in.marker ? in.marker : RENDERABLE_MARKERS;
    marker_t marker_out = {};
    marker_out.type = marker_mid;
    if (marker_out.type == RENDERABLE_MARKERS)
    {
        strcpy(marker_out.expression, "\\${\\w*\\([^$]*\\)}\\$");
        marker_out.size = 4;
        marker_out.front_size = 2;
        marker_out.back_size = 2;
        marker_out.eval_increment = 1;
    }
    else
    { // marker_out.type == EXECUTABLE_MARKERS
        strcpy(marker_out.expression, "\\$E{\\w*\\([^$]*\\)}\\$");
        marker_out.size = 5;
        marker_out.front_size = 3;
        marker_out.back_size = 2;
        marker_out.eval_increment = 8;
    }
    return parse_html_core(raw_html_out, marker_out);
}

/** Find and execute any C function calls within the raw html.
    \param raw_html The HTML you want to parse. This should be of type html_t or
   char*. \return The HTML that is ready to be rendered.
*/
html_t parse_html_core(html_t raw_html, marker_t marker)
{
    regmatch_t M[1];

    regex_t regex;
    int re;
    re = regcomp(&regex, marker.expression, 0);
    re = regexec(&regex, raw_html, 1, M, 0);

    if (!re) // re == 0 if there is a match
    {
        int precision = (M[0].rm_eo - marker.size) - (M[0].rm_so);
        char to_eval[precision + marker.eval_increment + 1];
        char *evaled;
        if (marker.type == RENDERABLE_MARKERS)
        {
            sprintf(to_eval, "_%.*s", precision,
                    raw_html + M[0].rm_so + marker.front_size);
            evaled = (char *)emscripten_run_script_int(to_eval);
        }
        else // marker.type == EXECUTABLE_MARKERS
        {
            sprintf(to_eval, "Module._%.*s", precision,
                    raw_html + M[0].rm_so + marker.front_size);
            evaled = to_eval;
        }

        char html_until_marker[M[0].rm_so + 1];
        char html_after_marker[strlen(raw_html) - M[0].rm_eo + 1];

        sprintf(html_until_marker, "%.*s", (int)M[0].rm_so, raw_html);
        sprintf(html_after_marker, "%s", &raw_html[M[0].rm_eo]);

        char rendered_html[strlen(html_until_marker) + strlen(html_after_marker) + strlen(evaled) + 1];

        strcat(rendered_html, html_until_marker);
        strcat(rendered_html, evaled);
        strcat(rendered_html, html_after_marker);

        if (marker.type == RENDERABLE_MARKERS)
        {
            free(evaled);
        }
        return parse_html(rendered_html, RENDERABLE_MARKERS);
    }
    else if (re == REG_NOMATCH && marker.type == RENDERABLE_MARKERS)
    {
        return parse_html(raw_html, EXECUTABLE_MARKERS);
    }
    else if (re == REG_NOMATCH && marker.type == EXECUTABLE_MARKERS)
    {
        return raw_html;
    }
    else
    {
        html_t error_html =
#include "error.cml"
            ;

        return error_html;
    }
}

/** A wrapper for \c parse_html, and the JS funtion \c displayInner .

    \param raw_html The HTML you want to display. This should be of type html_t
   or char*.
*/
EMSCRIPTEN_KEEPALIVE
void display_html(html_t raw_html)
{
    html_t rendered_html = parse_html(raw_html);
    displayInner(rendered_html);
}
