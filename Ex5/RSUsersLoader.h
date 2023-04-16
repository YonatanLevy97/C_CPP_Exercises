#ifndef SCHOOL_SOLUTION_USERFACTORY_H
#define SCHOOL_SOLUTION_USERFACTORY_H
#include <vector>
#include "RSUser.h"
#include "RecommenderSystem.h"
using std::shared_ptr;
using RS = RecommenderSystem;
using RSU_vec = std::vector<RSUser>;
class RSUsersLoader
{
private:

public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users and their movie
     * ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static RSU_vec create_users_from_file(const string& users_file_path,
                       std::unique_ptr<RecommenderSystem> rs) noexcept(false);
};


#endif //SCHOOL_SOLUTION_USERFACTORY_H
