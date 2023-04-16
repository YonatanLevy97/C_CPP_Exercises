#include "RecommenderSystem.h"
#include <cmath>
using std::isnan;
using std::make_shared;
using std::make_pair;
using std::pair;
using std::vector;
using std::map;



d_vec sum_vecs( d_vec& v1, const d_vec& v2)
{
    bool is_not_init = v1.empty();
    if (!is_not_init && v1.size() != v2.size()) { return v1; }
    for(size_t i = 0; i < v2.size(); i++)
    {
        if (is_not_init)
        {
            v1.push_back(v2[i]);
            continue;
        }
        v1[i] += v2[i];
    }
    return v1;
}

double vec_norm(const d_vec& vec)
{
    double norm = 0;
    for (const double v : vec)
    {
        norm += (v*v);
    }
    return sqrt(norm);
}

double mult_sum_vecs(const d_vec& vec1, const d_vec& vec2)
{
    if (vec1.size() != vec2.size()) { throw std::length_error("");}
    double sum_dots = 0;
    for(size_t i = 0; i < vec1.size(); ++i)
    {
        sum_dots += (vec1[i] * vec2[i]);
    }
    return sum_dots;
}

void RecommenderSystem::rec_by_content_stage_1(rank_map& user_ranks)
{
    double average = 0;
    int num_of_ranked_movies = 0;
    for (const auto& movie : user_ranks)
    {
        if(isnan(movie.second)){ continue; }
        average += movie.second;
        num_of_ranked_movies ++;
    }
    average = (num_of_ranked_movies > 0) ? (average/num_of_ranked_movies) : 0;
    for (auto& movie : user_ranks)
    {
        if(!isnan(movie.second)) { movie.second -= average; };
    }
}

d_vec RecommenderSystem::rec_by_content_stage_2(rank_map& user_ranks)
{
    movies_map prefer_map(_movies);
    d_vec prefer_vec;
    for (auto& movie : user_ranks)
    {
        if(isnan(movie.second)) { continue; }
        for (auto& i: prefer_map[movie.first]) { i *= movie.second;}
        sum_vecs(prefer_vec, prefer_map[movie.first]);
    }
    return prefer_vec;
}

sp_movie RecommenderSystem::rec_by_content_stage_3(rank_map& user_ranks,
                                                   d_vec& prefer_vec)
{
    pair<sp_movie, double> res = {nullptr, 0};
    for (auto& movie : user_ranks)
    {
        if(!isnan(movie.second)) { continue;}
        d_vec movie_feature = _movies[movie.first];
        double rank = mult_sum_vecs(prefer_vec, movie_feature);
        double rank_machane = vec_norm(prefer_vec) * vec_norm(movie_feature);
        if (rank_machane == 0) {throw std::runtime_error(""); }
        rank /= rank_machane;
        pair<sp_movie, double> movie_rank = {movie.first, rank};
        if (res.first == nullptr || res.second < rank){ res = movie_rank; }
    }
    return res.first;
}


sp_movie RecommenderSystem::recommend_by_content(const RSUser &user)
{
    rank_map user_ranks = user.get_ranks();
    rec_by_content_stage_1(user_ranks);
    d_vec prefer_vec = rec_by_content_stage_2(user_ranks);
    return rec_by_content_stage_3(user_ranks, prefer_vec);
}

sp_movie RecommenderSystem::add_movie(const string &name, int year,
                                      const vector<double> &features)
{
    sp_movie to_add = get_movie(name, year);
    if (to_add == nullptr)
    {
        to_add = make_shared<Movie>(name, year);
    }
    _movies.insert(make_pair(to_add,features));
    return to_add;
}

sp_movie RecommenderSystem::get_movie(const string &name, int year) const
{
    sp_movie to_find = make_shared<Movie>(name, year);
    auto found = _movies.find(to_find);
    if (found == _movies.end()) { return nullptr; }
    return found->first;
}

ostream &operator<<(ostream &os, const RecommenderSystem &rs)
{
    for(const auto& movie : rs._movies)
    {
        os << (*movie.first);
    }
    return os;
}

double RecommenderSystem::predict_movie_score(const RSUser &user,
                                              const sp_movie &movie, int k)
{
    rank_map user_ranks = user.get_ranks();
    d_vec movie_features = _movies[movie];
    double movie_norm = vec_norm(movie_features);
    map<double, sp_movie> ranks;
    for (auto & mov_rank : user_ranks)
    {
        if(isnan(mov_rank.second)){ continue; }
        sp_movie movie_with_rank = mov_rank.first;
        double im = mult_sum_vecs(movie_features, _movies[movie_with_rank]);
        double im_machane = movie_norm * vec_norm(_movies[movie_with_rank]);
        if (im_machane == 0) {throw std::runtime_error(""); }
        im /= im_machane;
        ranks[im] = movie_with_rank;
    }
    int i = 0;
    double res_mone = 0;
    double res_machane = 0;
    for (auto it = ranks.rbegin(); it != ranks.rend(); it++)
    {
        if (i >= k) {break;}
        res_machane += it->first;
        res_mone += (it->first * user_ranks[it->second]);
        i++;
    }
    if (res_machane == 0) {throw std::runtime_error(""); }
    return (res_mone / res_machane);
}

sp_movie RecommenderSystem::recommend_by_cf(const RSUser &user, int k)
{
    rank_map user_ranks = user.get_ranks();
    map<double, sp_movie> ranks_rec;
    for (auto & v : user_ranks)
    {
        if(!isnan(v.second)) { continue; }
        double rk = predict_movie_score(user, v.first, k);
        ranks_rec[rk] = v.first;
    }
    return (ranks_rec.rbegin())->second;
}
