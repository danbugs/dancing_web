/** \file 
    Macros, types, and function signatures that are not intended to be used by the end-user.
*/
#ifndef DCW_INTERNAL_H
#define DCW_INTERNAL_H
#include "dcw.h"

/** The two types of marker currently supported by DCW.
    - RENDERABLE_MARKERS = markers that enclose content meant to be rendered on the DOM.
    - EXECUTABLE_MARKERS = Markers that enclose content meant to be executed at runtime.
*/
enum marker_type
{
    RENDERABLE_MARKERS,
    EXECUTABLE_MARKERS,
};

/** The type passed as an argument to the \c parse_html_core function. 
    It contains all necessary information on a marker to aid rendering.
*/
typedef struct
{
    char expression[20]; // maximum 20 chars (using this instead of char* to avoid a null-pointer-dereference)
    enum marker_type type;
    int size;
    int front_size;
    int back_size;
    int eval_increment; 
    // ^ how many characters we need to add to the expression when evaluated 
    // (i.e., 1 for RENDERABLE_MARKERS due to the '_' , and 8 for EXECUTABLE_MARKERS due to the 'Module._')
} marker_t;

/** The type passed as an argument to the \c parse_html_wrapper function. This
    is part of what enables default arguments values for \c parse_html_core.
*/
typedef struct
{
    html_t raw_html;
    enum marker_type marker;
} parse_html_args;

/** A wrapper around \c parse_html_wrapper to allow calling \c parse_html_core
 * indirectly.
 */
#define parse_html(...) parse_html_wrapper((parse_html_args){__VA_ARGS__});

html_t parse_html_wrapper(parse_html_args in);

html_t parse_html_core(html_t raw_html, marker_t marker);

void display_html(html_t raw_html);
#endif