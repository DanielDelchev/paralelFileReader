#ifndef COMMANDPARAMETERS_H_INCLUDED
#define COMMANDPARAMETERS_H_INCLUDED
#include <string>

struct commandParameters{
    bool quiet = false;
    bool test = false;
    int threads = 1;
    const char* file = "451.txt";
    std::string testResultsFile = "";
};


#endif // COMMANDPARAMETERS_H_INCLUDED
