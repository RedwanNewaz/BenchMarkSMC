
# SMC BENCHMARKING 
In this project, I am going to show how SMC improves the performance of planning algorithms. 
To guide this project, I will separately define the problem domain.

## Target problem domain

* Navigation domain
* Hallway domain
* Rock sample domain

## Dealing with hyper-parameter 
Hyper parameter is crucial part of any algorithm. There are two ways to deal with hyper parameters.
* JSON library 
* TINY xml library 

Tiny xml is intuitive library for parsing xml file. However, I need to write more code to load parameters.
On the other hand, JSON parameter server is easy to use but because of lacking typedef hard to understand. 

### MAP file
This is going to be a text file input for navigation problem which represents obstacle and free space location. 
However, for hallway problem I may need to use different map parser. The reason MAP folder will be under the sub directory of 
Parameter folder is some hyper-parameter file may include map description. 

## Algorithm 
There are several algorithms that in my mind to implement 
* POMCP         
* despotPlanner
* RAMCP    

However, RAMCP does not support navigation problem. I need to implement navigation problem definition for RAMCP. 
It is important to note that algorithm should be implemented as a shared_ptr class and all the algorithm related parameter should
be initialized inside each class. Don't depend on solver class to initialize algorithm parameter.  
## Sovler

Solver takes two inputs 
* parameter file 
* model checker 

Since in this case, SMC structure does not change with respect to the problem definition, model checker could be a boolean flag. 

## Dealing with libraries 

There are two important libraries 
* AIToolbox 
* despotPlanner 

AIToolbox is a rich libraries which includes several POMDP planning related algorithms. Supporting AIToolbox with model checker 
will help me to include their rich implementation planning algorithms. However, AIToolbox does not support despotPlanner which is important 
comparison for this work. Either I will use AIToolbox as an super class for despotPlanner or subclass for despotPlanner.  # BenchMarkSMC
