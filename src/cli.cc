/*
    hole uploader
    Copyright (C) 2022 xwashere

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <argp.h>
};

#include <string>

#include "hole.h"

enum struct pth_op {
    INVAL, UPLOAD
};

struct pth_args {
    pth_op op = pth_op::INVAL;

    std::string url;

    char*       key = 0;

    char*       ekey = 0;
    std::string group;

    std::string filename;

    int __parg = 0;
};

const char* argp_program_version = "0.0.1";
argp argp_data = {
    .options = (argp_option[]){
        { .flags = OPTION_DOC, .doc = "operations:" },
        {
            .name  = "upload",
            .key   = 'U',
            .doc   = "put an object in the hole",
        },
        { .flags = OPTION_DOC, .doc = "generic:" },
        {
            .name  = "key",
            .key   = 'k',
            .arg   = "key",
            .doc   = "A documentation string for this option, for printing in help messages.",
        },
        { .name  = "key", .flags = OPTION_HIDDEN },
        { .flags = OPTION_DOC, .doc = "upload:"},
        {
            .name  = "group",
            .key   = 'g',
            .arg   = "group",
            .doc   = "group to upload to"
        },
        { .name  = "group", .flags = OPTION_HIDDEN },
        {
            .name  = "ekey",
            .key   = 'e',
            .arg   = "ekey",
            .doc   = "encryption key"
        },
        { .name  = "ekey", .flags = OPTION_HIDDEN },
        {}
    },
    .parser = [](int key, char* arg, argp_state* state) -> error_t {
        pth_args* pstate = (pth_args*)state->input;

        switch (key) {
            case 'U': 
                if (pstate->op != pth_op::INVAL) return 1;
                pstate->op = pth_op::UPLOAD;
                break;
            case 'k':
                pstate->key = strdup(arg);
                break;
            case 'g':
                pstate->group = arg;
                break;
            case 'e':
                pstate->ekey = strdup(arg);
                break;
            case ARGP_KEY_ARG:
                if      (pstate->__parg == 0) pstate->url = arg;
                else if (pstate->__parg == 1) pstate->filename = arg;
                pstate->__parg++;
                break;
            default:
                return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }
};

int main(int argc, char** argv) {
    pth_args args;

    switch (argp_parse(&argp_data, argc, argv, 0, NULL, &args)) {
        case 1: fprintf(stderr, "conflicting ops\n");
                exit(1);
    }

    hole::hole_global_init();
    atexit(hole::hole_global_fini);

    hole::hole_client cl(args.url, args.key);

    if (args.op == pth_op::UPLOAD) {
                              printf("upload \"%s\":\n", args.url.c_str());
        if (args.key)         printf(" key: %s\n", args.key);
                              printf(" file: %s\n", args.filename.c_str());
        if (args.group != "") printf(" group: %s\n", args.group.c_str());
        if (args.ekey)        printf(" encryption key: %s\n", args.ekey);

        hole::hole_object res;
        cl.create_object(res, args.filename, args.group, args.ekey);
    }

    if (args.ekey) {
        explicit_bzero(args.ekey, strlen(args.ekey));
        free(args.ekey);
    }

    if (args.key) {
        explicit_bzero(args.key, strlen(args.key));
        free(args.key);
    }
}