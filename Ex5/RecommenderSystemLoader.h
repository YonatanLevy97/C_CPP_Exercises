
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"
using std::string;
using std::unique_ptr;
typedef unique_ptr<RecommenderSystem> unique_rs;
using RS = RecommenderSystem;

class RecommenderSystemLoader {

 private:

 public:
  RecommenderSystemLoader () = delete;
  /**
   * loads movies by the given format for movies with their feature's score
   * @param movies_file_path a path to the file of the movies
   * @return smart pointer to a RecommenderSystem which was created with
   * those movies
   */
  static unique_rs create_rs_from_movies_file (
                        const string &movies_file_path) noexcept (false);
};

#endif //RECOMMENDERSYSTEMLOADER_H
