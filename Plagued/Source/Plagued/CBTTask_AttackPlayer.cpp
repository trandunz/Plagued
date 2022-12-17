// Fill out your copyright notice in the Description page of Project Settings.


#include "CBTTask_AttackPlayer.h"

#include "AIController.h"
#include "CCZombie.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/GameSession.h"

UCBTTask_AttackPlayer::UCBTTask_AttackPlayer()
{
	NodeName = TEXT("Attack Player");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UCBTTask_AttackPlayer, BlackboardKey));
}

EBTNodeResult::Type UCBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();
	auto Origin  = AIPawn->GetActorLocation();

	if (ACCZombie* zombie = Cast<ACCZombie>(AIController->GetCharacter()))
	{
		zombie->Attack();
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UCBTTask_AttackPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("Attack Something"));
}