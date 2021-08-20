/**
 * @license
 * Copyright 2021 Danilo Chiarlone
 * SPDX-License-Identifier: GNU AFFERO GENERAL PUBLIC LICENSE
*/

#define html_t char *
#define RAW_HTML(...) #__VA_ARGS__;

void displayInner(html_t html);
char* render_html(char* html);
void display(html_t raw_html);
