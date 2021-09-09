#include <stdlib.h>
#include <stdio.h>
#include <emscripten.h>
#include "../dependencies/dcw/dcw.h"

extern void remove_html_with_class(char *class_name);
extern void display_html(html_t raw_html);

typedef struct
{
    char *first_name;
    char *last_name;
} FormInput;

FormInput finput;

html_t form_h = HTMLIFY(" \
<form action='/action_page.php'> \
  <label for='fname'>First name:</label><br> \
  <input type='text' id='fname' name='fname' oninput='$E{set_first_name(Module.allocateUTF8(this.value))}$'><br> \
  <label for='lname'>Last name:</label><br> \
  <input type='text' id='lname' name='lname' oninput='$E{set_last_name(Module.allocateUTF8(this.value))}$'><br><br> \
</form>  \
<button onclick='$E{form_submit()}$'>Submit Form</button> \
");

html_t display_h = HTMLIFY(" \
<div class='display'> \
    <p>First Name: ${get_first_name()}$</p> \
    <p>Last Name: ${get_last_name()}$</p> \
</div> \
");

EMSCRIPTEN_KEEPALIVE
void set_first_name(char* value) {
    asprintf(&(finput.first_name), "%s", value);
    free(value);
}

EMSCRIPTEN_KEEPALIVE
html_t get_first_name() {
    return finput.first_name;
}

EMSCRIPTEN_KEEPALIVE
void set_last_name(char* value) {
    asprintf(&(finput.last_name), "%s", value);
    free(value);
}

EMSCRIPTEN_KEEPALIVE
html_t get_last_name() {
    return finput.last_name;
}

EMSCRIPTEN_KEEPALIVE
void form_submit(char* value) {
    remove_html_with_class("display");
    display_html(display_h);
}


int main() {
    display_html(form_h);
}