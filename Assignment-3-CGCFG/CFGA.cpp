// /**
//  * ICFG.cpp
//  * @author kisslune 
//  */

// #include "CFGA.h"

// using namespace SVF;
// using namespace llvm;
// using namespace std;

// int main(int argc, char **argv)
// {
//     auto moduleNameVec =
//             OptionBase::parseOptions(argc, argv, "Whole Program Points-to Analysis",
//                                      "[options] <input-bitcode...>");

//     LLVMModuleSet::buildSVFModule(moduleNameVec);

//     SVFIRBuilder builder;
//     auto pag = builder.build();
//     auto icfg = pag->getICFG();

//     CFGAnalysis analyzer = CFGAnalysis(icfg);

//     // TODO: complete the following method: 'CFGAnalysis::analyze'
//     analyzer.analyze(icfg);

//     analyzer.dumpPaths();
//     LLVMModuleSet::releaseLLVMModuleSet();
//     return 0;
// }


// void CFGAnalysis::analyze(SVF::ICFG *icfg)
// {
//     // Sources and sinks are specified when an analyzer is instantiated.
//     for (auto src : sources)
//         for (auto snk : sinks)
//         {
//             // TODO: DFS the graph, starting from src and detecting the paths ending at snk.
//             // Use the class method 'recordPath' (already defined) to record the path you detected.
//             //@{

//             //@}
//         }
// }




/**
 * ICFG.cpp
 * Assignment 3 - ICFG Path Analysis
 * Name: 胡晨璐
 * Student ID: 3220252746
 */

#include "CFGA.h"
#include <functional>

using namespace SVF;
using namespace llvm;
using namespace std;

int main(int argc, char **argv)
{
    auto moduleNameVec =
            OptionBase::parseOptions(argc, argv, "Whole Program Points-to Analysis",
                                     "[options] <input-bitcode...>");

    LLVMModuleSet::buildSVFModule(moduleNameVec);

    SVFIRBuilder builder;
    auto pag = builder.build();
    auto icfg = pag->getICFG();

    CFGAnalysis analyzer = CFGAnalysis(icfg);

    // TODO: complete the following method: 'CFGAnalysis::analyze'
    analyzer.analyze(icfg);

    analyzer.dumpPaths();
    LLVMModuleSet::releaseLLVMModuleSet();
    return 0;
}


// void CFGAnalysis::analyze(SVF::ICFG *icfg)
// {
//     // Sources and sinks are specified when an analyzer is instantiated.
//     for (auto src : sources)
//         for (auto snk : sinks)
//         {
//             // TODO: DFS the graph, starting from src and detecting the paths ending at snk.
//             // Use the class method 'recordPath' (already defined) to record the path you detected.
//             //@{

//             //@}
//         }
// }

void CFGAnalysis::analyze(SVF::ICFG *icfg)
{
    std::function<void(unsigned, unsigned, std::vector<unsigned>&, std::set<unsigned>&)> dfs;
    
    dfs = [&](unsigned current, unsigned sink, 
              std::vector<unsigned> &path, std::set<unsigned> &visited) -> void
    {
        path.push_back(current);
        visited.insert(current);
        
        if (current == sink)
        {
            recordPath(path);
            path.pop_back();
            visited.erase(current);
            return;
        }
        
        auto node = icfg->getICFGNode(current);
        
        if (auto callNode = llvm::dyn_cast<SVF::CallICFGNode>(node))
        {
            callStack.push(current);
            
            for (auto edge : node->getOutEdges())
            {
                unsigned succ = edge->getDstID();
                if (visited.find(succ) == visited.end())
                {
                    dfs(succ, sink, path, visited);
                }
            }
            
            if (!callStack.empty())
                callStack.pop();
        }
        else if (auto retNode = llvm::dyn_cast<SVF::RetICFGNode>(node))
        {
            for (auto edge : node->getOutEdges())
            {
                unsigned succ = edge->getDstID();
                if (visited.find(succ) == visited.end())
                {
                    dfs(succ, sink, path, visited);
                }
            }
        }
        else
        {
            for (auto edge : node->getOutEdges())
            {
                unsigned succ = edge->getDstID();
                if (visited.find(succ) == visited.end())
                {
                    dfs(succ, sink, path, visited);
                }
            }
        }
        
        path.pop_back();
        visited.erase(current);
    };
    
    for (auto src : sources)
    {
        for (auto snk : sinks)
        {
            std::vector<unsigned> path;
            std::set<unsigned> visited;
            dfs(src, snk, path, visited);
        }
    }
}
