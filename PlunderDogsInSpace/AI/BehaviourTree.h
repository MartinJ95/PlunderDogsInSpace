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

class BTDecoratorNode : public BTNode
{
public:
	virtual BTNodeResult Evaluate(void* ptr = nullptr) override = 0;
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

