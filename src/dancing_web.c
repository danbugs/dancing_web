/**
 * @license
 * Copyright 2021 Danilo Chiarlone
 * SPDX-License-Identifier: GNU AFFERO GENERAL PUBLIC LICENSE
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <emscripten.h>

#include "dancing_web.h"

EM_JS(void, displayInner, (html_t html),
      {
          var element = document.createElement('div');

          if (element)
              element.innerHTML = ""; // clear browser cache
          if (element)
          {
              element.innerHTML += UTF8ToString(html);
          }
          document.body.appendChild(element);
      });
char *render_html(char *html)
{
    regmatch_t M[1];

    regex_t regex;
    int re;
    re = regcomp(&regex, "\\${\\w*\\([^$]*\\)}\\$", 0);
    re = regexec(&regex, html, 1, M, 0);

    if (!re)
    {
        int precision = (M[0].rm_eo - 4) - (M[0].rm_so);
        char to_eval[128];
        sprintf(to_eval, "_%.*s", precision, html + M[0].rm_so + 2);
        int evaled_pointer = emscripten_run_script_int(to_eval);
        sprintf(to_eval, "UTF8ToString(%d)", evaled_pointer);
        char *evaled = emscripten_run_script_string(to_eval);
        free(evaled_pointer);

        char rendered_html[strlen(html) - (M[0].rm_eo - M[0].rm_so) + strlen(evaled) + 1];
        char html_until_marker[M[0].rm_so + 1];
        char html_after_marker[strlen(html) - M[0].rm_eo + 1];

        sprintf(html_until_marker, "%.*s", (int)M[0].rm_so, html);
        sprintf(html_after_marker, "%s", &html[M[0].rm_eo]);

        strcat(rendered_html, html_until_marker);
        strcat(rendered_html, evaled);
        strcat(rendered_html, html_after_marker);

        return render_html(rendered_html);
    }
    else if (re == REG_NOMATCH)
    {
        return html;
    }
    else
    {
        html_t error_html =
#include "error.cml"

            return error_html;
    }
}

void display(html_t raw_html)
{
    html_t rendered_html = render_html(raw_html);
    displayInner(rendered_html);
}
