// Fill out your copyright notice in the Description page of Project Settings.


#include "Aim_Notify.h"
#include "PlaguedCharacter.h"

void UAim_Notify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
		if (APlaguedCharacter* character = Cast<APlaguedCharacter>(MeshComp->GetOwner()))
		{
			character->CanMeleeAttack = true;
		}
}

void UAim_Notify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
}
