//
// Created on 2/20/2022.
//

// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"


// implement your cpp code here
RSUser::RSUser(const string name, rank_map& ranks, std::shared_ptr<RS>& rs)
{
    _rs = rs;
    _ranks_map = std::move(ranks);
    _username = name;
}

rank_map RSUser::get_ranks() const
{
    return _ranks_map;
}

string RSUser::get_name() const
{
    return _username;
}

void RSUser::add_movie_to_rs(const string &name, int year,
                         const std::vector<double> &features, double rate)
{
    sp_movie movie =_rs->add_movie(name, year, features);
    _ranks_map.insert(std::make_pair(movie, rate));
}

sp_movie RSUser::get_recommendation_by_content() const
{
    return _rs->recommend_by_content(*this);
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
    return _rs->recommend_by_cf(*this, k);
}

double RSUser::get_prediction_score_for_movie(const string &name, int year,
                                              int k) const
{
    sp_movie movie = std::make_shared<Movie>(name, year);
    return _rs->predict_movie_score(*this, movie, k);
}

ostream &operator<<(ostream &os, const RSUser& rs_user)
{
    os << "name: " << rs_user.get_name() << "\n";
    std::map<sp_movie,double, cmp_movies> order_map;
    for (const auto& pair : rs_user._ranks_map)
    {
        order_map[pair.first] = pair.second;
    }
    for(const auto& pair : order_map)
    {
        os << (*pair.first);
    }
    os << std::endl;
    return os;
}



