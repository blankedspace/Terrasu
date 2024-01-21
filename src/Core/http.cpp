#include "http.h"


namespace Terrasu {
    json search(std::string query, std::function<void(json)> callback) {
        std::string url = "http://localhost:8080/search";
        httplib::Client cli("http://localhost:8080");

        // Perform the GET request to download the MP3 file
        httplib::Params params;
        params.emplace("query", query);
        httplib::Headers headers;
        auto res = cli.Get(url, params, headers);
        json data = json::parse(res->body);
        callback(data["items"]);

        return data["items"];
        

    }

    std::string download(std::string video_id, std::function<void(std::string)> callback) {
        std::string url = "http://localhost:8080/download";
        httplib::Client cli("http://localhost:8080");
       
        // Perform the GET request to download the MP3 file
        httplib::Params params;
        params.emplace("video_id", video_id);
        httplib::Headers headers;
        auto res = cli.Get(url, params, headers);
        if (res && res->status == 200) {
            // Save the MP3 data to a file
            auto name = res->get_header_value("SongName");
            std::ofstream out_file("Assets/music/" + name + ".mp3", std::ios::binary);
            out_file.write(res->body.c_str(), res->body.length());
            out_file.close();
            std::cout << "MP3 file downloaded successfully!" << std::endl;
            callback(name);
            return name;
        }
        else {
            std::cerr << "Failed to download MP3 file: " << res.error() << std::endl;
            callback("");
            return "";
        }

    }
}