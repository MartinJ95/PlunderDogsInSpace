#include "BehaviourTree.h"

BTNodeResult BTSelectorNode::Evaluate()
{
    for (BTNode*& n : m_children)
    {
        if (n->Evaluate() == BTNodeResult::eBTSuccess)
        {
            return BTNodeResult::eBTSuccess;
        }
    }
    return BTNodeResult::eBTFail;
}

BTNodeResult BTSequenceNode::Evaluate()
{
    for (BTNode*& n : m_children)
    {
        if (n->Evaluate() == BTNodeResult::eBTFail)
        {
            return BTNodeResult::eBTFail;
        }
    }
    return BTNodeResult::eBTSuccess;
}
