//
// Created on 2/20/2022.
//

#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include <algorithm>
#include <map>
using std::ostream;
struct cmp_movies
{
    bool operator()(const sp_movie& m1, const sp_movie& m2) const
    {
        return (*m1 < *m2);
    }
};

typedef std::vector<double> d_vec;
typedef std::map<sp_movie, d_vec, cmp_movies> movies_map;

d_vec sum_vecs(const d_vec& v1, const d_vec& v2);
double vec_norm(const d_vec& vec);
double mult_sum_vecs(const d_vec& vec1, const d_vec& vec2);

class RecommenderSystem
{
public:
	explicit RecommenderSystem() = default;;
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,
                       const std::vector<double>& features);


    /**
     * a function that calculates the movie with highest score based on movie
     * features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const RSUser& user);

    /**
     * a function that calculates the movie with highest predicted score based
     * on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const RSUser& user, int k);


    /**
     * Predict a user rating for a movie given argument using item cf procedure
     * with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const RSUser &user,const sp_movie& movie,int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;

    friend ostream& operator<<(ostream& os, const RecommenderSystem& rs);

private:
    void rec_by_content_stage_1(rank_map& user_ranks);
    d_vec rec_by_content_stage_2(rank_map& user_ranks);
    sp_movie rec_by_content_stage_3(rank_map& user_ranks, d_vec& prefer_vec);
    movies_map _movies;
};


#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
