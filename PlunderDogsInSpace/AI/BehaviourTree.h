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
	virtual BTNodeResult Evaluate() = 0;
};

class BTSelectorNode : public BTNode
{
public:
	virtual BTNodeResult Evaluate() override;
	std::vector<BTNode*>& GetChildren();
	void AddChild(BTNode*&& NewNode);
protected:
	std::vector<BTNode*> m_children;
};

class BTSequenceNode : public BTSelectorNode
{
public:
	virtual BTNodeResult Evaluate() override final;
};

class BTDecoratorNode : public BTNode
{
public:
	virtual BTNodeResult Evaluate() override = 0;
};


class BehaviourTree
{
public:
	BehaviourTree(BTSelectorNode*&& node);
	~BehaviourTree();
public:
	BTNodeResult Evaluate();
	BTSelectorNode* GetRoot();
protected:
	BTSelectorNode* m_root;
};

