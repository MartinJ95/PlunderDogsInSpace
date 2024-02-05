#pragma once
#include <vector>

enum class BTNodeResult
{
	eBTSuccess,
	eBTFail,
	eBTRunning
};

class BTNode
{
public:
	virtual BTNodeResult Evaluate(void* ptr = nullptr) = 0;
};

class BTSelectorNode : public BTNode
{
public:
	virtual BTNodeResult Evaluate(void* ptr = nullptr) override;
	std::vector<BTNode*>& GetChildren();
	void AddChild(BTNode*&& NewNode);
	~BTSelectorNode();
protected:
	std::vector<BTNode*> m_children;
};

class BTSequenceNode : public BTSelectorNode
{
public:
	virtual BTNodeResult Evaluate(void* ptr = nullptr) override final;
};

struct BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const = 0;
};

class BTDecoratorNode : public BTNode
{
public:
	BTDecoratorNode(const BTAction* Action) : m_action(Action) {}
	virtual BTNodeResult Evaluate(void* ptr = nullptr) override;
public:
	const BTAction* m_action;
};


class BehaviourTree
{
public:
	BehaviourTree(BTSelectorNode*&& node);
	BehaviourTree(const BehaviourTree& other);
	BehaviourTree(BehaviourTree&& other);
	void operator=(const BehaviourTree& other)
	{
		m_root = other.m_root;
	}
	void operator=(BehaviourTree&& other)
	{
		m_root = other.m_root;
		other.m_root = nullptr;
	}
	~BehaviourTree();
public:
	BTNodeResult Evaluate(void* ptr = nullptr);
	BTSelectorNode* GetRoot();
protected:
	BTSelectorNode* m_root;
};

