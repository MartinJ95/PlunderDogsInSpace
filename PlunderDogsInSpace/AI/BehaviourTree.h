#pragma once
#include <vector>
#include <string>
#include <unordered_map>

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

class BehaviourState;

struct BehaviourCustomState
{
	virtual void ConstructState(std::unordered_map<std::string, BehaviourState>& States) const = 0;
};

static std::vector<BehaviourCustomState*> BehaviourStateObjs;

class BehaviourState
{
public:
	BehaviourState(std::vector<BehaviourTree>&& Trees);
	BehaviourState(const BehaviourState& other);
	BehaviourState(BehaviourState&& other);
	void operator=(const BehaviourState& other)
	{
		trees = other.trees;
	}
	void operator=(BehaviourState&& other)
	{
		trees = std::move(other.trees);
	}
	void EvaluateState(void* ptr = nullptr);
	static void InitStates()
	{
		for (std::vector<BehaviourCustomState*>::iterator it = BehaviourStateObjs.begin(); it != BehaviourStateObjs.end(); it++)
		{
			BehaviourCustomState* ref = *it;
			ref->ConstructState(States);
		}
	}
	static BehaviourState& GetState(const std::string& StateName);
private:
	std::vector<BehaviourTree> trees;
	static std::unordered_map<std::string, BehaviourState> States;
};