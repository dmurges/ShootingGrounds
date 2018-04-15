// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type	UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{



	auto aiController = OwnerComp.GetAIOwner();
	auto controlledPawn = aiController->GetPawn();
	auto patrolRoute = controlledPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(patrolRoute)) { return EBTNodeResult::Failed; }

	auto patrolPoints = patrolRoute->getPatrolPoints();
	if (patrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A guard is miising point"));
		return EBTNodeResult::Failed;
	}


	//set next waypoint
	auto blackboardComp = OwnerComp.GetBlackboardComponent();
	auto index = blackboardComp->GetValueAsInt(indexKey.SelectedKeyName);
	blackboardComp->SetValueAsObject(waypointKey.SelectedKeyName, patrolPoints[index]);
	
	auto nextIndex = (index + 1) % patrolPoints.Num();
	blackboardComp->SetValueAsInt(indexKey.SelectedKeyName, nextIndex);
	return EBTNodeResult::Succeeded;
}
