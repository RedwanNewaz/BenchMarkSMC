#include <iostream>
#include "despot/Setup.h"
#include "dialog.h"
#include <QApplication>
#include <cassert>
#include "Planner/despotPlanner.h"

int main(int argc, char **argv) {

    QApplication a(argc, argv);
    // check argument
    assert(argc==2 && "Missing parameter file");
    // read a JSON file
//    auto parm = make_shared<Parameter>(argv[1]);
//    auto param = parm->get();
//    int depth;
//    string solver;
//    param->getParam("solver", &solver);
//    param->getParam("depth",&depth);
//
//    auto res = param->getDepotArg(argv[0]);
//    debug(res.first<<"\t");
//
//    debug(depth <<"\t"<< solver);


    Dialog w;
    despotPlanner dp;
    dp.start();

    w.show();
    return a.exec();
}