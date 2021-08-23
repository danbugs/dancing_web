# DanCing Web 💃🕸 (DCW)

## Running the example

- Prerequisites:
    - Emscripten (to install, check [this](https://emscripten.org/docs/getting_started/downloads.html) out), and
    - An HTTP Server (e.g., I'm using Python's `http.server` module).

- Now, from inside the example folder, execute:
    - `emcc hello_world.c ../src/dancing_web.c` to compile, and
    - `python3 -m http.server` to run.

To view it, navigate to `http://localhost:8000/` on a browser.

## Usage

If you are using an IDE, to avoid C auto-formatting the HTML to something that doesn't work, you can create separate files for it. These files can have any extension but I've been using `.cml` (i.e., C Markup Language) — they look like this:

File `hello_world.cml`:
```
RAW_HTML(
<div>
  <h1 class="red">${hello_world()}$</h1>
  <p>1 + 142856 = ${add_two_numbers(1, 142856)}$</p>
</div>
<style>
  .red {
    color: red;
  }
</style>
);
```

> - Note: To get code auto-formatting working for .cml files on VSCode, open `settings.json` and add the following at the end of it:
>> ```
>> "files.associations": {
>>     "*.cml": "html"
>> },
>> ```
> - Refer to the `settings.json` file in the .vscode folder.

You can call C functions within the `${` and `}$` markers. These functions will be executed and the result will display in the client.

That said, within C, you can use this `hello_world.cml` like so:

File `public/hello_world.c`:
```
// ...
html_t html = 
    #include "public/hello_world.cml"
// ...
```

- Things to note:
    - `#include "error.cml"` must be on a newline, and
    - the `#include "error.cml"` stated does not have a terminating semi-colon (i.e., ";").

Otherwise, you can write HTML directly in your C file with:
```
html_t html = RAW_HTML(
<div>
  <h1 class="red">${hello_world()}$</h1>
  <p>1 + 142856 = ${add_two_numbers(1, 142856)}$</p>
</div>
<style>
  .red {
    color: red;
  }
</style>
);
```

After writting your HTML, you can render it by calling the `display` function:

```
// ...
display(html);
// ...
```

Overall, a basic example looks like this:

```
#include <emscripten.h>
#include "../src/dancing_web.h"

EMSCRIPTEN_KEEPALIVE // this is compulsory for functions you intend to call from HTML
html_t add_two_numbers(int a, int b) // functions that return content to be rendered must return the html_t type
{
    // this example illustrates how you can get a another type (i.e., int) to be returned as html_t
    char *tmp = malloc(128 * sizeof(int)); // don't worry about free-ing this malloc, the framework will free it after rendering
    int result = a + b;
    sprintf(tmp, "%d", result); // this works because html_t expands to char*
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
        #include "public/hello_world.cml"

    display(html);
}
```

After this, just create a basic root `index.html` file to call our JavaScript:
```
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <link rel="icon" href="./public/favicon.ico" />
    <title>DanCing Web Example</title>
  </head>
  <body>
    <script async type="text/javascript" src="./a.out.js"></script>
  </body>
</html>
```

## How to use DanCing Web in your project?

- The way I'd recommend doing it, is:
    - if your project isn't already a `git` repository, run: `git init`, and
    - next, run: `git submodule add https://github.com/danbugs/dancing_web.git`.

- After this:
  - add: `#include "dancing_web/src/dancing_web.h"` at the top of `your_file.c` and refer to the usage section above on how to get started,
  - to compile, run: `emcc your_file.c ./dancing_web/src/dancing_web.c`, and
  - to finish off, create the basic root `index.html` file I mentioned in the usage section.

- To update the dependency, run: `git submodule update --remote --merge`.

