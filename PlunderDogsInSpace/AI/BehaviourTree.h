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
};

