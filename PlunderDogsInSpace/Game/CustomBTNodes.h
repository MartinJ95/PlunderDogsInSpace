#pragma once
#include "../AI/BehaviourTree.h"

struct BTShipFindTarget : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTShipSetMoveToLocation : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTShipMoveToLocation : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTShipShootAtTarget : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTShipLookingAtTarget : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTShipRotateToTarget : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTSucceed : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTHasGotTarget : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTSetPhysicsMoveForward : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTSetPhysicsMoveBackword : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

struct BTApplyPhysics : public BTAction
{
	virtual BTNodeResult Evaluate(void* ptr = nullptr) const override;
};

constexpr BTShipFindTarget BTShipFindTargetObj;
constexpr BTShipSetMoveToLocation BTShipSetMoveToLocationObj;
constexpr BTShipMoveToLocation BTShipMoveToLocationObj;
constexpr BTShipShootAtTarget BTShipShootAtTargetObj;
constexpr BTShipLookingAtTarget BTShipLookingAtTargetObj;
constexpr BTShipRotateToTarget BTShipRotateToTargetObj;
constexpr BTSucceed BTSucceedObj;
constexpr BTHasGotTarget BTHasGotTargetObj;

struct AIFindingTarget : public BehaviourCustomState
{
	virtual void ConstructState(std::unordered_map<std::string, BehaviourState>& States) const override final;
};

struct ShootAtTarget : public BehaviourCustomState
{
	virtual void ConstructState(std::unordered_map<std::string, BehaviourState>& States) const override final;
};

struct MoveToTargetocation : public BehaviourCustomState
{
	virtual void ConstructState(std::unordered_map<std::string, BehaviourState>& States) const override final;
};

constexpr AIFindingTarget AIFindingTargetObj;
constexpr ShootAtTarget ShootAtTargetObj;