// Fill out your copyright notice in the Description page of Project Settings.


#include "Jump_Notify.h"
#include "PlaguedCharacter.h"

void UJump_Notify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (APlaguedCharacter* character = Cast<APlaguedCharacter>(MeshComp->GetOwner()))
	{
		character->Jump();
	}
}
