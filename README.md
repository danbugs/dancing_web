# DanCing Web 💃🕸 (DCW)

## Getting Started

Dancing Web is now distributed with the [Tarantella Package Manager](https://github.com/danbugs/tarantella) — a tool I've made to simplify setup of projects like these! 

To install the Tarantella Package Manager, you need the Rust toolchain — for instructions on how to install it, see [this](https://www.rust-lang.org/tools/install). Once you're done with that, you can install Tarantella by running: `cargo install tarantella`.

To start-off, setup a new main module WASM project with: `tapm new my-first-project`. This will start a new C WASM project with:
- a build folder, 
- a dependencies folder,
- an index.html,
- a `Makefile`,
- a releases folder, 
- a src folder with some starting code in `main.c`, and a
- `Tarantella.toml` file.

Next, add the latest version of Dancing Web as a dependency with: `tapm add "danbugs/dancing_web"`. This will automatically download DCW to your dependencies folder, add it to your `Tarantella.toml` list of dependencies, and append `dependencies/dcw/dcw.o` to the `DEPENDENCIES` variable of your `Makefile` to facilitate compilation.

Now, let's create our first HTML-equivalent "C Markdown Language" file (`.cml`) to be rendered from C! First, create a folder called `frontend/` and, inside it, create a `hello_world.cml` file with the following code:

```HTML
HTML(
<div>
  <h1 class="red">${hello_world()}$</h1>
</div>
<style>
  .red {
    color: red;
  }
</style>
);
```

You can call C functions within the `${` and `}$` markers (called renderable markers) or within the `$E` and `}$` markers (called executable markers).
  - Renderable markers are for C functions that return content that is meant to be rendered, and
  - Executable markers are for C functions that are meant to be called but don't necessarily render HTML (e.g., functions within a button's `onclick`).

Next, in our `main.c`, replace its' contents with:

```C
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include "../dependencies/dcw/dcw.h"

extern void display_html(html_t raw_html);
// ^^^ rendering function from DCW that is 
// external to our main module

                     //    this is compulsory for functions
EMSCRIPTEN_KEEPALIVE // <- you intend to call from HTML
html_t hello_world()
{
    // ^^^ functions that return content to be rendered 
    // must return the html_t or char* types

    html_t tmp = malloc(128 * sizeof(char));
    // ^^^ you don't need to free this malloc,
    // the framework will do it for you.

    sprintf(tmp, "%s", "Hello, World!");
    // ^^^ this works because html_t expands 
    // to char*

    return tmp;
}
// ^^^ this is the hello_world function 
// we are calling from our .cml file


int main()
{
    html_t html =
#include "../frontend/hello_world.cml"
    ;
    // ^^^ this brings the .cml component into scope!
    // things to note:
    // - the #include statement must be in a line of its' 
    // own, and
    // - the ';' is optional but, if present, must also be 
    // on a line of its' own 
    // (usually, I like to include the ';' because it 
    // helps with formatting)

    display_html(html);
    // ^^^ this will render our html.
}
```

To finish off, in your `Makefile`, append `--js-library dependencies/dcw/dcw.js` to your `EMCC_FLAGS` variable. It should look like this: `EMCC_CFLAGS=-s MAIN_MODULE=1 --js-library dependencies/dcw/dcw.js`.

Now, let's build the project with: `tapm build`. This will use the `Makefile` to compile the project to the `build/` folder.

To test the project, run: `tapm run`. Navigating to `http://127.0.0.1:4000` in your browser, you should see:

![getting-started-1](https://i.imgur.com/zGWqSow.png)

FOr a more complex example, view the project in the `example/` folder of the repo!

## Appendix

### Why use `.cml` files?

If you are using an IDE, to avoid C auto-formatting the HTML to something that doesn't work, you can create separate files for it. These files can have any extension but I've been using `.cml` (i.e., C Markup Language).

In addition, to get code auto-formatting working for `.cml` files on VSCode, open `settings.json` and add the following at the end of it:

```
"files.associations": {
    "*.cml": "html"
},
```

### An alternative to `.cml` files

You can write HTML directly within C, like this:

```
html_t html = HTML(
<div>
  <h1 class="red">${hello_world()}$</h1>
</div>
<style>
  .red {
    color: red;
  }
</style>
);
```

### Building from source

The root level `Makefile` was designed for POSIX-compliant systems — `tapm build` will not work on Windows outside of WSL.

### Binding a C function to a DOM event

![binding-a-c-function-to-a-dom-event](https://camo.githubusercontent.com/339f5dbb4a8a9945034f5b1ba5efc7e5e2780a5353361c2fbea52b5ae47c7c4b/68747470733a2f2f692e696d6775722e636f6d2f6c766e6f646e442e676966)

### Returning other types as `html_t`

```
EMSCRIPTEN_KEEPALIVE
html_t add_two_numbers(int a, int b)
{
    char *tmp = malloc(128 * sizeof(int));
    int result = a + b;
    sprintf(tmp, "%d", result);
    html_t expands to char*
    return tmp;
}
```

### Returning unmalloc-ed items

For the record, a function like:

```
EMSCRIPTEN_KEEPALIVE
html_t hello_world()
{
    return "Hello, World!";
}
```
... would also work. Thing is, it is not the best because the framework will try to free a pointer that wasn't malloc-ed. While this doesn't cause an error, I wouldn't call it a best practice.

### Are you updating HTML you've already rendered?

If that's the case, you must remove the old html before you update your state. For example, when updating the counter in `example/src/hello_world.c`, I do:

```C
EMSCRIPTEN_KEEPALIVE
void increase_counter()
{
    remove_html(main_h);
    counter++;
    display_html(main_h);
}
```

### Notes to Self

To check for any problems (i.e., mem leaks and whatnot) in the code at runtime, compile like so: `emcc hello_world.c ../../src/dcw.c --js-library ../../src/dcw.js -fsanitize=address -s ALLOW_MEMORY_GROWTH -s INITIAL_MEMORY=285212672 -gsource-map --source-map-base http://127.0.0.1:4000`. After that, run with: `tapm run`.
