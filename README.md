# DanCing Web 💃🕸 (DCW)

## Running the example

- Prerequisites:

  - Emscripten (to install, check [this](https://emscripten.org/docs/getting_started/downloads.html) out), and
  - An HTTP Server (e.g., I'm using Python's `http.server` module).

- Now, do the following:
  - copy the build folder to the example folder,
  - run `emmake make` to compile, and
  - `python3 -m http.server` to run.

To view it, navigate to `http://localhost:8000/` on a browser.

> Note: I've experienced issues compiling the example code in WSL — I'll be investigating this in the future.

## Usage

If you are using an IDE, to avoid C auto-formatting the HTML to something that doesn't work, you can create separate files for it. These files can have any extension but I've been using `.cml` (i.e., C Markup Language) — they look like this:

File `hello_world.cml`:

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
> - Alternatively, refer to the `settings.json` file in the .vscode folder of this git repository.

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

> Note: I'm hoping to automate a lot of this in the future w/ a CLI tool.

## How to use DanCing Web in your project?

### Using gitsubmodules

- To get started:
  - if your project isn't already a `git` repository, run: `git init`, and
  - next, run: `git submodule add https://github.com/danbugs/dancing_web.git`.

- After this:

  - add: `#include "dancing_web/src/dcw.h"` at the top of `<your_file>.c` and refer to the usage section above on how to get started,
  - to compile, run: `emcc <your_file>.c dancing_web/src/dcw.c --js-library dancing_web/src/dancing_web.js`, and
  - to finish off, create the basic root `index.html` file I mentioned in the usage section.

- To update the dependency, run: `git submodule update --remote --merge`.

### Using dynamic linking

- To start off, download the latest release of the repository:
![dynamicLinkingDownload](https://i.imgur.com/L8LfX17.gif)

- Unzip the file in the root of your project and delete the `.zip` file.

- Next, to dynamically link your main module to DCW, create a file called `pre.js` like so:

File `pre.js`:

```
Module['dynamicLibraries'] = ['/dcw.wasm'];
```

- Next, create a `Makefile` like so: to facilitate compilation:

File `Makefile`:

```
P=<your_file>
OBJECTS=<your_file>.c
EMCC=emcc
EMCC_CFLAGS=-s MAIN_MODULE=1 --pre-js pre.js --js-library dcw-latest/dcw.js

$(P): $(OBJECTS)
	$(EMCC) $(P).c $(EMCC_CFLAGS)
```

- At the top of `<your_file>.c`, add:

File `<your_file>.c`
```
#include "dcw-latest/dcw.h"

extern void display_html(html_t raw_html);
```

- To finish off, create the basic root `index.html` file I mentioned in the usage section.
