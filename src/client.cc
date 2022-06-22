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

#include <stdio.h>

#include <curl/curl.h>

#include "client.h"
#include "object.h"

namespace hole {
    hole_client::hole_client(const std::string& url, const char* key) 
        : __key(key) {
        {
            int i = url.length() - 1;
            for (; i >= 0 && url[i] == '/'; i--); 
            __url = url.substr(0, i + 1);
        }

        if (!(__session = curl_easy_init())) {
            fprintf(stderr, "catastrophic failure\n");
            abort();
        }
    }

    hole_client::~hole_client() {
        curl_easy_cleanup(__session);
    }

    int hole_client::create_object(hole_object& object, const std::string& filename, const std::string& group, const char* ekey) {
        curl_mime* mime = curl_mime_init(__session);
            curl_mimepart* content = curl_mime_addpart(mime);
                curl_mime_name    (content, "content");
                curl_mime_filedata(content, filename.c_str());
            curl_mimepart* grp = curl_mime_addpart(mime);
                curl_mime_name(grp, "group");
                curl_mime_data(grp, group.c_str(), group.length());
        
        curl_slist* list = curl_slist_append(0, (std::string("Authorization: ") + __key).c_str());

        curl_easy_setopt(__session, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(__session, CURLOPT_URL,        (__url + "/api/objects").c_str());
        curl_easy_setopt(__session, CURLOPT_POST,       1);
        curl_easy_setopt(__session, CURLOPT_MIMEPOST,   mime);

        CURLcode res = curl_easy_perform(__session);

        printf("%i\n", res);

        curl_mime_free(mime);
        curl_slist_free_all(list);

        curl_easy_reset(__session);

        return 0;
    }
}