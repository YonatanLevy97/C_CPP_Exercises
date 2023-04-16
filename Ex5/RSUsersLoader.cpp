#include <map>
#include <fstream>
#include <sstream>
#include "RSUsersLoader.h"
#include <cmath>

using std::cout;

/**
 * replace char c with '\0' in case c inside str.
 * @param str string to replace c letter.
 * @param c char c to replace with '\0'.
 */
void replace_in_nedded(string& str, char c)
{
    if(str.find(c) != string::npos)
    {
        str.replace(str.find(c),1, "\0");
    }
}

/**
 * extracting movies from file to sp_movie vector.
 * @param is std::ifstream&
 * @param movies std::vector<sp_movie>&
 * @param rs_sh std::shared_ptr<RS>&
 */
void extract_movies_from_file(std::ifstream& is, std::vector<sp_movie>& movies,
                              std::shared_ptr<RS>& rs_sh)
{
    string line;
    getline(is, line);
    std::istringstream iss ( line );
    while(getline ( iss, line, ' '))
    {
        auto kama_i = line.find('-');
        string name = line.substr(0 , kama_i);
        string year_str = line.substr(kama_i+1, line.length());
        int year = std::stoi(year_str, nullptr);
        movies.push_back(rs_sh->get_movie(name,year));
    }
}

RSU_vec RSUsersLoader::create_users_from_file(const string &users_file_path,
                   std::unique_ptr<RS> rs) noexcept(false)
{
    std::ifstream is(users_file_path);
    if (!is.is_open()) {throw std::runtime_error("");};
    std::shared_ptr<RS> rs_sh = std::move(rs);
    RSU_vec users;
    string line;
    std::vector<sp_movie> movies;
    extract_movies_from_file(is,movies,rs_sh);

    while (getline(is, line))
    {
        std::istringstream in_line ( line );
        string name;
        rank_map ranks(0, sp_movie_hash, sp_movie_equal);
        for(int i = 0; getline(in_line, line, ' '); i ++)
        {
            replace_in_nedded(line, '\n'), replace_in_nedded(line, '\r');
            if ( i == 0) { name = line; continue; }
            sp_movie movie = movies[i-1];
            if (line == "NA") { ranks[movie] = NAN; continue; }
            ranks[movie] = std::stod(line);
        }
        RSUser user(name, ranks, rs_sh);
        users.push_back(user);
    }
    return users;
}
