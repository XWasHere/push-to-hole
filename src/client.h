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

#pragma once

#include <curl/curl.h>
#include <string>

#include "object.h"

namespace hole {
    struct hole_client {
        hole_client(const std::string& url, const char* key);
        ~hole_client();

        int create_object(hole_object& object, const std::string& filename, const std::string& group, const char* ekey = 0);

        std::string __url;
        const char* __key = 0;
        CURL*       __session = 0;
    };
}