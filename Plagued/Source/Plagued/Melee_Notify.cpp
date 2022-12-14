// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee_Notify.h"
#include "PlaguedCharacter.h"

void UMelee_Notify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (APlaguedCharacter* character = Cast<APlaguedCharacter>(MeshComp->GetOwner()))
	{
		character->TryMeleeAttack = false;
		character->CanMeleeAttack = false;
	}
}

void UMelee_Notify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (APlaguedCharacter* character = Cast<APlaguedCharacter>(MeshComp->GetOwner()))
	{
		character->CanMeleeAttack = true;
	}
}
