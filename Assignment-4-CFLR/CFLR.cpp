/**
 * CFLR.cpp
 * @author kisslune 
 */

#include "A4Header.h"

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
    pag->dump("pag");

    CFLR solver;
    solver.buildGraph(pag);
    // TODO: complete this method
    solver.solve();
    solver.dumpResult();

    LLVMModuleSet::releaseLLVMModuleSet();
    return 0;
}


// CFLR.cpp - 补全solve函数

void CFLR::solve()
{
    // 初始化worklist，将所有初始边加入
    for (auto& srcItr : graph->getSuccessorMap()) {
        unsigned src = srcItr.first;
        for (auto& lblItr : srcItr.second) {
            EdgeLabel label = lblItr.first;
            for (auto dst : lblItr.second) {
                workList.push(CFLREdge(src, dst, label));
            }
        }
    }

    // 主循环：处理worklist直到为空
    while (!workList.empty()) {
        CFLREdge edge = workList.pop();
        
        // 处理一元规则
        processUnaryRules(edge);
        
        // 处理二元规则
        processBinaryRules(edge);
    }
}

void CFLR::processUnaryRules(const CFLREdge& edge)
{
    // 规则1: PT -> Addr
    if (edge.label == Addr) {
        addNewEdge(edge.src, edge.dst, PT);
    }
    
    // 规则2: SV -> Load
    if (edge.label == Load) {
        addNewEdge(edge.src, edge.dst, SV);
    }
    
    // 规则3: PV -> Store  
    if (edge.label == Store) {
        addNewEdge(edge.src, edge.dst, PV);
    }
    
    // 规则4: VP -> StoreBar
    if (edge.label == StoreBar) {
        addNewEdge(edge.src, edge.dst, VP);
    }
    
    // 规则5: VF -> LoadBar
    if (edge.label == LoadBar) {
        addNewEdge(edge.src, edge.dst, VF);
    }
    
    // 规则6: VA -> AddrBar
    if (edge.label == AddrBar) {
        addNewEdge(edge.src, edge.dst, VA);
    }
    
    // 规则7: LV -> CopyBar
    if (edge.label == CopyBar) {
        addNewEdge(edge.src, edge.dst, LV);
    }
}

void CFLR::processBinaryRules(const CFLREdge& edge)
{
    // 获取前驱和后继映射以便快速查找
    auto& predMap = graph->getPredecessorMap();
    auto& succMap = graph->getSuccessorMap();
    
    // 规则8: PT -> Copy PT
    if (edge.label == Copy) {
        for (auto& ptEdge : succMap[edge.dst]) {
            if (ptEdge.first == PT) {
                for (auto k : ptEdge.second) {
                    addNewEdge(edge.src, k, PT);
                }
            }
        }
    }
    
    // 规则9: PT -> PT Copy
    if (edge.label == PT) {
        for (auto& copyEdge : succMap[edge.dst]) {
            if (copyEdge.first == Copy) {
                for (auto k : copyEdge.second) {
                    addNewEdge(edge.src, k, PT);
                }
            }
        }
    }
    
    // 规则10: PT -> SV PT
    if (edge.label == SV) {
        for (auto& ptEdge : succMap[edge.dst]) {
            if (ptEdge.first == PT) {
                for (auto k : ptEdge.second) {
                    addNewEdge(edge.src, k, PT);
                }
            }
        }
    }
    
    // 规则11: PT -> PT PV
    if (edge.label == PT) {
        for (auto& pvEdge : succMap[edge.dst]) {
            if (pvEdge.first == PV) {
                for (auto k : pvEdge.second) {
                    addNewEdge(edge.src, k, PT);
                }
            }
        }
    }
    
    // 规则12: PT -> VP PT
    if (edge.label == VP) {
        for (auto& ptEdge : succMap[edge.dst]) {
            if (ptEdge.first == PT) {
                for (auto k : ptEdge.second) {
                    addNewEdge(edge.src, k, PT);
                }
            }
        }
    }
    
    // 规则13: PT -> PT VF
    if (edge.label == PT) {
        for (auto& vfEdge : succMap[edge.dst]) {
            if (vfEdge.first == VF) {
                for (auto k : vfEdge.second) {
                    addNewEdge(edge.src, k, PT);
                }
            }
        }
    }
    
    // 规则14: SV -> Load PT
    if (edge.label == Load) {
        for (auto& ptEdge : succMap[edge.dst]) {
            if (ptEdge.first == PT) {
                for (auto k : ptEdge.second) {
                    addNewEdge(edge.src, k, SV);
                }
            }
        }
    }
    
    // 规则15: PV -> PT Store
    if (edge.label == PT) {
        for (auto& storeEdge : succMap[edge.dst]) {
            if (storeEdge.first == Store) {
                for (auto k : storeEdge.second) {
                    addNewEdge(edge.src, k, PV);
                }
            }
        }
    }
    
    // 规则16: VP -> StoreBar PT
    if (edge.label == StoreBar) {
        for (auto& ptEdge : succMap[edge.dst]) {
            if (ptEdge.first == PT) {
                for (auto k : ptEdge.second) {
                    addNewEdge(edge.src, k, VP);
                }
            }
        }
    }
    
    // 规则17: VF -> PT LoadBar
    if (edge.label == PT) {
        for (auto& loadBarEdge : succMap[edge.dst]) {
            if (loadBarEdge.first == LoadBar) {
                for (auto k : loadBarEdge.second) {
                    addNewEdge(edge.src, k, VF);
                }
            }
        }
    }
    
    // 规则18: VA -> PT AddrBar
    if (edge.label == PT) {
        for (auto& addrBarEdge : succMap[edge.dst]) {
            if (addrBarEdge.first == AddrBar) {
                for (auto k : addrBarEdge.second) {
                    addNewEdge(edge.src, k, VA);
                }
            }
        }
    }
    
    // 规则19: LV -> PT CopyBar
    if (edge.label == PT) {
        for (auto& copyBarEdge : succMap[edge.dst]) {
            if (copyBarEdge.first == CopyBar) {
                for (auto k : copyBarEdge.second) {
                    addNewEdge(edge.src, k, LV);
                }
            }
        }
    }
}

void CFLR::addNewEdge(unsigned src, unsigned dst, EdgeLabel label)
{
    // 检查边是否已存在
    if (!graph->hasEdge(src, dst, label)) {
        // 添加新边到图
        graph->addEdge(src, dst, label);
        // 将新边加入worklist继续处理
        workList.push(CFLREdge(src, dst, label));
    }
}
