#include <string>
#include <iterator>
#include <vector>
#include <stdexcept>
#include "HashMap.hpp" // should i do this?

using std::string;
using strvec = std::vector<string>;
using std::pair;
using iterator = std::vector<pair<string,string>>::const_iterator;

class InvalidKey: public std::invalid_argument {
public:
    explicit InvalidKey(const string& msg_err) :invalid_argument(msg_err){};
    InvalidKey () : InvalidKey("Invalid key argument"){};
};

class Dictionary : public HashMap<string,string> {
public:
    Dictionary() = default;
    Dictionary(const strvec& keys, const strvec& values):
                                        HashMap<string,string>(keys, values){};
    virtual bool erase(const string& key) override
    {
        if(!HashMap<string,string>::contains_key(key)){ throw InvalidKey(); }
        return HashMap<string,string>::erase(key);
    }
    bool update(const iterator& it_begin, const iterator& it_end)
    {
        for (auto begin = it_begin; begin != it_end; begin++)
        {
            const string& key = begin->first;
            if(HashMap<string,string>::contains_key(key))
            {
                HashMap<string,string>::at(key)=begin->second;
            }
            else if(!
                (HashMap<string,string>::insert(begin->first,begin->second)))
            {
                return false;
            }
        }
        return true;
    }
};