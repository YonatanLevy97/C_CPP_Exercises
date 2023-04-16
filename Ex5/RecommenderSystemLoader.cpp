#include "RecommenderSystemLoader.h"
#include <fstream>
#include <vector>



unique_rs RecommenderSystemLoader::create_rs_from_movies_file(
                            const string& movies_file_path) noexcept(false)
{
    std::ifstream is(movies_file_path);
    if (!is.is_open()) {throw std::runtime_error(""); };
    unique_rs rs = std::make_unique<RS>();
    string fullname, kama = "-";
    int year;
    double a, b, c, d;
    while (is.good())
    {
        is >> fullname >> a >> b >> c >> d;
        if (!is.good()) { break; }
        unsigned int kama_i = fullname.find(kama);
        string name(fullname.substr(0,kama_i));
        string year_str = fullname.substr(kama_i+1, fullname.length());
        year = std::stoi(year_str, nullptr);
        rs->add_movie(name, year, {a,b,c,d});
    }
    is.close();
    return rs;
}
