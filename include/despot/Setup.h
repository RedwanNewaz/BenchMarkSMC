//
// Created by redwan on 4/6/19.
//

#ifndef BENCHMARKSMC_SETUP_H
#define BENCHMARKSMC_SETUP_H
#include <ostream>
#include <fstream>
#include <string>
#include <typeinfo>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;


class Parameter:public std::enable_shared_from_this<Parameter>{
    typedef std::shared_ptr<Parameter> ParamPtr;
public:
    Parameter(const char *filename) : filename_(filename) {
        std::ifstream input(filename);
        input >> param_;
    }

    ParamPtr get(){
        return shared_from_this();
    }

    template <typename T>
    void getParam( string inp, T* out){
        // json class automatically convert type
        *out = param_["dependencies"][inp.c_str()];
    }

    friend ostream &operator<<(ostream &os, const Parameter &parameter) {
        os  << " filename_: "
            << parameter.filename_ << " param_: " << parameter.param_;
        return os;
    }

    pair<int,char **> getDepotArg(char* file){


        vector<string> param_int{"runs","simlen","depth","seed","timeout","noise","nparticles","prune","solver","world"};

        char *ArgVal[param_int.size()];
        for (int j = 0; j < param_int.size(); j++) {
            string sim_param;
            if(j<param_int.size()-2){
                int val;
                getParam(param_int[j],&val);
                sim_param = to_string(val);
            }
            else{
                string val;
                getParam(param_int[j],&val);
                sim_param = val;
            }

            ArgVal[j] = new char[sim_param.length()+1];
            strcpy(ArgVal[j], sim_param.c_str());

        }


        char *customArg[]={
                file,
                "--runs",       ArgVal[1],
                "--simlen",     ArgVal[2],
                "--depth",      ArgVal[3],
                "--seed",       ArgVal[4],
                "--timeout",    ArgVal[5],
                "--noise",      ArgVal[6],
                "--nparticles", ArgVal[7],
                "--prune",      ArgVal[8],
                "--solver",     ArgVal[9],
                "--world",      ArgVal[10]
        };
        int  argc = sizeof(customArg)/ sizeof(customArg[0]);
        return make_pair(argc, customArg);
    }


private:
    const char *filename_;
    json param_;
};

#endif //BENCHMARKSMC_SETUP_H
