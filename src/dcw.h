#pragma once
/** \file 
    Useful macros intended to be used by the end-user.
*/

/*
 * Copyright 2021 Danilo Chiarlone
 * LICENSE: GNU AFFERO GENERAL PUBLIC LICENSE
*/

/** A macro that expands to the \c char* type — created to distinguish between normal strings and those that are meant to be rendered to the DOM.
*/
#define html_t char *

/** A function macro to allow embedding HTML within C files.
*/
#define HTML(...) #__VA_ARGS__;

/** A utility function for easy debug. It prints the variable name and its' value.
    Usage:
    char *hello_world = "Hello, World!";
    dbg(hello_world, %d);

    Output:
    "hello_world: Hello, World!"
*/
#define dbg(to_print, formatter)                          \
    {                                                     \
        printf(#to_print ": " #formatter "\n", to_print); \
    }