//
// Created by redwan on 4/6/19.
//

#ifndef BENCHMARKSMC_SHAREDDATA_H
#define BENCHMARKSMC_SHAREDDATA_H

#include <iostream>
#include <memory>
#include <QtCore>
#include <QPoint>
#include <unordered_map>
#include <QDebug>
#include <QMutex>
#include <QVector>
using namespace std;
static QMutex MUTE;


class SharedData:public enable_shared_from_this<SharedData>{
    typedef shared_ptr<SharedData> DataPtr;
    typedef  QVector<double >V;
public:

    bool showSim = false;
    int count_;

    SharedData(){
        dis_reward_ = 0;
        count_ =0;
    }
    DataPtr getPtr(){
        return shared_from_this();
    }

    void updateSize(int sz)const{
        MUTE.lock();
        const_cast<SharedData*>(this)->size_= sz;
        MUTE.unlock();
    }

    /**
     * map size is required for visualizing the simulation grid
     * @return grid size is square, need only one parameter
     */
    int mapSize(){ return size_;}

    void updateMap(unordered_map<int,int>&map)const{


        MUTE.lock();
        // perform const_cast to populate member variables
        for(auto it=map.begin();it !=map.end();++it){
            auto data = getCoord(it->first);
            if(it->second==1){

                const_cast<SharedData*>(this)->robot= data;
            }
            else if(it->second==2){
                const_cast<SharedData*>(this)->sample.push_back(data);
            }
            else if(it->second==3){
                const_cast<SharedData*>(this)->rock.push_back(data);
            }
        }
        MUTE.unlock();


    }
    /**@brief
     * get map for simulation environment visualization
     */

    void getMap(V& sample_x,V& sample_y,V& rock_x, V&rock_y){

        foreach(auto v,sample){
            if(v==robot)
                continue;
            sample_x.push_back(v.x()+0.5);
            sample_y.push_back(v.y()+0.5);
        }
        foreach(auto v,rock){
            if(v==robot)
                continue;
            rock_x.push_back(v.x()+0.5);
            rock_y.push_back(v.y()+0.5);
        }

        sample.clear();
        rock.clear();
    }


    QPoint getRobotPos(){ return robot; }

    void getRobotPos(QVector<double>&x, QVector<double>&y ){
        x.push_back(robot.x()+0.5);
        y.push_back(robot.y()+0.5);

    }

public:
    /**     *
     * @param val1 : discounted reward
     * @param val2  undiscounted reward which needs to integrate over step
     * @param count will keep track of steps
     */


    void updateValue(double val1, double val2){
        MUTE.lock();
        dis_reward_ += val1;
        ++count_;
        qDebug()<<"dis "<< dis_reward_<<" undisc "<<val2;
        disReward_.push_back(dis_reward_);
        undisReward_.push_back(val2);
        stepCount_.push_back(count_);
        showSim = true;
        MUTE.unlock();

    }
    /**@brief
     * get reward for performance visualization
     */
    void getReward(V& x,V& y0,V& y1){

        std::copy(stepCount_.begin(),stepCount_.end(),back_inserter(x));
        std::copy(disReward_.begin(),disReward_.end(),back_inserter(y0));
        std::copy(undisReward_.begin(),undisReward_.end(),back_inserter(y1));

    }

    void toggleSim(){
        MUTE.lock();
        showSim=false;
        MUTE.unlock();
    }




protected:
    QPoint getCoord(int x)const{
        return QPoint(x%size_,x/size_);
    }

private:
    int size_;
    QPoint robot;
    QVector<QPoint> sample, rock;
    double dis_reward_;
    QVector<double> disReward_, undisReward_, stepCount_;


};



#endif //BENCHMARKSMC_SHAREDDATA_H
