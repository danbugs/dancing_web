# DanCing Web 💃🕸 (DCW)

## Running the example

- Prerequisites:

  - Emscripten (to install, check [this](https://emscripten.org/docs/getting_started/downloads.html) out), and
  - An HTTP Server (e.g., I'm using Python's `http.server` module).

- Now, do the following:
  - at the root of the project, run `emmake make`* to compile the side module,
  - copy the dcw_latest folder from the root of the project to inside the example folder,
  - run `emmake make` from inside the example folder to compile the main module, and
  - run `python3 -m http.server` to start the application.

To view it, navigate to `http://localhost:8000/` on a browser.

> Note: I've experienced issues compiling the example code in WSL — I'll be investigating this in the future.

> *: The root level `Makefile` was designed for POSIX-compliant systems. To run it on Windows, you might have to do:
>> ```
>> bash
>> emmake make
>> exit
>> ```
> - If you get that "'bash' is not recognized as an internal or external command", see [this](https://stackoverflow.com/questions/42438587/bash-is-not-recognized-as-an-internal-or-external-command) for ways you can run bash from Windows — I'm using WSL.

## Overview of the example

If you are using an IDE, to avoid C auto-formatting the HTML to something that doesn't work, you can create separate files for it. These files can have any extension but I've been using `.cml` (i.e., C Markup Language) — they look like this:

File `frontend/hello_world.cml`:

```
HTML(
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
>   > ```
>   > "files.associations": {
>   >     "*.cml": "html"
>   > },
>   > ```

You can call C functions within the `${` and `}$` markers. These functions will be executed and the result will display in the UI.

That said, within C, you can use this `hello_world.cml` like so:

File `public/hello_world.c`:

```
// ...
html_t html =
    #include "frontend/hello_world.cml"
// ...
```

- Things to note:
  - `#include "error.cml"` must be on a newline, and
  - the `#include "error.cml"` statement does not have a terminating semi-colon (i.e., ";").

Otherwise, you can write HTML directly in your C file with:

```
html_t html = HTML(
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

After writting your HTML, you can render it by calling the `display_html` function:

```
// ...
display_html(html);
// ...
```

Overall, a basic example looks like this:

```
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include "dcw_latest/dcw.h"

extern void display_html(html_t raw_html); // this is necessary for the compiler to know this function will be available at runtime

EMSCRIPTEN_KEEPALIVE // this is compulsory for functions you intend to call from HTML
html_t add_two_numbers(int a, int b) // functions that return content to be rendered must return the html_t or char* types
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
#include "frontend/hello_world.cml"

        display_html(html);
}
```

> Note: Look at the comments in the example above for deeper understanding of the underlying logic.

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

> Note: I'm hoping to automate a lot of this in the future w/ a CLI tool.

## How to use DanCing Web in your project?

### Using gitsubmodules

- To get started:

  - if your project isn't already a `git` repository, run: `git init`, and
  - next, run: `git submodule add https://github.com/danbugs/dancing_web.git`.

- After this:

  - At the top of `<your_file>.c`, add:

    File `<your_file>.c`

    ```
    #include "dancing_web/src/dcw.h"

    extern void display_html(html_t raw_html);
    ```

  - to compile, run: `emcc <your_file>.c dancing_web/src/dcw.c --js-library dancing_web/src/dcw.js`, and
  - To finish off, create the basic root `index.html` file I mentioned in the "Overview of the example" section.
  - Now, to run, execute: `python3 -m http.server`.

- To update the dependency, run: `git submodule update --remote --merge`.

### Using dynamic linking

- To start off, download the latest release of the repository:
  ![dynamicLinkingDownload](https://i.imgur.com/L8LfX17.gif)

- Unzip the file in the root of your project and delete the `.zip` file.

- Next, to dynamically link your main module to DCW, create a file called `pre.js` like so:

File `pre.js`:

```
Module['dynamicLibraries'] = ['dcw_latest/dcw.wasm'];```

- Next, create a `Makefile` like so: to facilitate compilation:

File `Makefile`:

```
P=<your_file>
OBJECTS=<your_file>.c
EMCC=emcc
EMCC_CFLAGS=-s MAIN_MODULE=1 --pre-js pre.js --js-library dcw_latest/dcw.js

$(P): $(OBJECTS)
	$(EMCC) $(P).c $(EMCC_CFLAGS)
```

- At the top of `<your_file>.c`, add:

File `<your_file>.c`

```
#include "dcw_latest/dcw.h"

extern void display_html(html_t raw_html);
```

- To finish off, create the basic root `index.html` file I mentioned in the usage section.

- To compile, execute: `emmake make`.
- To run, execute: `python3 -m http.server`.
