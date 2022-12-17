// Fill out your copyright notice in the Description page of Project Settings.


#include "CBTTask_GoTo.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/GameSession.h"

UCBTTask_GoTo::UCBTTask_GoTo()
{
	NodeName = TEXT("Go To Location");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UCBTTask_GoTo, BlackboardKey));
}

EBTNodeResult::Type UCBTTask_GoTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location{};
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();
	auto Origin  = AIPawn->GetActorLocation();

	auto NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FVector closestPlayerLoc = {99999, 99999, 99999};
	for( FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator )
	{
		if (Iterator->Get()->IsLocalController())
		{
			if (closestPlayerLoc.Length() > FVector::Distance(Iterator->Get()->K2_GetActorLocation(), Origin))
			{
				closestPlayerLoc = Iterator->Get()->K2_GetActorLocation();
			}
			
		}
	}
	Location.Location = closestPlayerLoc;

	if (IsValid(NavSystem))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UCBTTask_GoTo::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector %s"), *BlackboardKey.SelectedKeyName.ToString());
}
