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

std::vector<BTNode*>& BTSelectorNode::GetChildren()
{
    return m_children;
}

void BTSelectorNode::AddChild(BTNode*&& NewNode)
{
    m_children.emplace_back(std::move(NewNode));
}

BTSelectorNode::~BTSelectorNode()
{
    for (BTNode* s : m_children)
    {
        delete s;
    }
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

BehaviourTree::BehaviourTree(BTSelectorNode*&& node) : m_root(std::move(node))
{
}

BehaviourTree::~BehaviourTree()
{
    delete m_root;
}

BTNodeResult BehaviourTree::Evaluate()
{
    return m_root->Evaluate();
}

BTSelectorNode* BehaviourTree::GetRoot()
{
    return m_root;
}
