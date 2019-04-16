#include <string>
#include <any>
#include <map>

#ifndef MUTATION_H
#define MUTATION_H

struct Mutation {
    virtual void mutate(std::string &contents, std::map<std::string, std::any> options) = 0;
    virtual std::string to_string(std::map<std::string, std::any> options) = 0;
};

#endif

