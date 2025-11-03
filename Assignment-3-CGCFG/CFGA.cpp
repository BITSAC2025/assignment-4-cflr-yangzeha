/**
 * ICFG.cpp
 * @author kisslune 
 */
#include "CFGA.h"
// 新增必要头文件：嵌套递归、ICFG节点/边定义（补充ICFGEdge头文件）
#include <functional>
#include "Graphs/ICFG.h"
#include "Graphs/ICFGEdge.h"  // 新增：确保ICFGEdge类型被正确识别

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
    analyzer.analyze(icfg);

    analyzer.dumpPaths();
    LLVMModuleSet::releaseLLVMModuleSet();
    return 0;
}

// 核心实现：DFS遍历ICFG（适配SVF旧版接口）
void CFGAnalysis::analyze(SVF::ICFG *icfg)
{
    for (auto src : sources)
    {
        for (auto snk : sinks)
        {
            vector<unsigned> currentPath;
            set<unsigned> visitedNodes;

            // 嵌套DFS递归函数
            function<void(unsigned)> dfs = [&](unsigned currentNodeId) {
                currentPath.push_back(currentNodeId);
                visitedNodes.insert(currentNodeId);

                // 到达main出口，记录路径
                if (currentNodeId == snk)
                {
                    this->recordPath(currentPath);
                }
                else
                {
                    // 关键修改：用getOutEdges()替代getSuccs()（SVF旧版接口）
                    ICFGNode* currentNode = icfg->getICFGNode(currentNodeId);
                    // getOutEdges()返回当前节点的所有出边，直接遍历边指针
                    for (ICFGEdge* succEdge : currentNode->getOutEdges())
                    {
                        // 从边中获取目标节点ID（逻辑不变）
                        unsigned nextNodeId = succEdge->getDstNode()->getId();
                        if (visitedNodes.find(nextNodeId) == visitedNodes.end())
                        {
                            dfs(nextNodeId);
                        }
                    }
                }

                // 回溯：恢复路径和访问状态
                currentPath.pop_back();
                visitedNodes.erase(currentNodeId);
            };

            dfs(src);  // 从main入口开始DFS
        }
    }
}