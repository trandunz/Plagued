// Fill out your copyright notice in the Description page of Project Settings.


#include "CBTTask_GetRandomLocation.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/GameSession.h"

UCBTTask_GetRandomLocation::UCBTTask_GetRandomLocation()
{
	NodeName = TEXT("Get Random Location Within Radius");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UCBTTask_GetRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UCBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location{};
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();
	auto Origin  = AIPawn->GetActorLocation();

	auto NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UCBTTask_GetRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector %s"), *BlackboardKey.SelectedKeyName.ToString());
}
