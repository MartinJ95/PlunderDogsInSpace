#include "BehaviourTree.h"
#include <cassert>

BTNodeResult BTSelectorNode::Evaluate(void* ptr)
{
    for (BTNode*& n : m_children)
    {
        if (n->Evaluate(ptr) == BTNodeResult::eBTSuccess)
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
    if (m_children.size() == 0)
        return;

    for (BTNode* s : m_children)
    {
        delete s;
    }
    m_children.clear();
}

BTNodeResult BTSequenceNode::Evaluate(void* ptr)
{
    for (BTNode*& n : m_children)
    {
        if (n->Evaluate(ptr) == BTNodeResult::eBTFail)
        {
            return BTNodeResult::eBTFail;
        }
    }
    return BTNodeResult::eBTSuccess;
}

BehaviourTree::BehaviourTree(BTSelectorNode*&& node) : m_root(std::move(node))
{
}

BehaviourTree::BehaviourTree(const BehaviourTree& other) : m_root(other.m_root)
{
}

BehaviourTree::BehaviourTree(BehaviourTree&& other) : m_root(other.m_root)
{
    other.m_root = nullptr;
}

BehaviourTree::~BehaviourTree()
{
    if (m_root == nullptr)
        return;

    delete m_root;
}

BTNodeResult BehaviourTree::Evaluate(void* ptr)
{
    return m_root->Evaluate(ptr);
}

BTSelectorNode* BehaviourTree::GetRoot()
{
    return m_root;
}

BTNodeResult BTDecoratorNode::Evaluate(void* ptr)
{
    return m_action->Evaluate(ptr);
}

std::unordered_map<std::string, BehaviourState> BehaviourState::States;

BehaviourState::BehaviourState(std::vector<BehaviourTree>&& Trees) : trees(std::move(Trees))
{
}

void BehaviourState::EvaluateState(void* ptr = nullptr)
{
    for (std::vector<BehaviourTree>::iterator it = trees.begin(); it != trees.end(); it++)
    {
        it->Evaluate(ptr);
    }
}

void BehaviourState::InitStates()
{
    for (std::vector<BehaviourCustomState*>::iterator it = BehaviourStateObjs.begin(); it != BehaviourStateObjs.end(); it++)
    {
        BehaviourCustomState* ref = *it;
        ref->ConstructState(States);
    }
}

BehaviourState& BehaviourState::GetState(const std::string& StateName)
{
    std::unordered_map<std::string, BehaviourState>::iterator state;
    state = States.find(StateName);

    assert((state != States.end()));

    return state->second;
}
