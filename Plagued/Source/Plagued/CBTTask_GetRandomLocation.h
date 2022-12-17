// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CBTTask_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class PLAGUED_API UCBTTask_GetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Search", meta = (AllowPrivateAccess = true))
	float SearchRadius{500.0f};
public:
	UCBTTask_GetRandomLocation();
};
