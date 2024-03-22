// Fill out your copyright notice in the Description page of Project Settings.


#include "ScienceQuestExamineOverlap.h"
#include "Components/SphereComponent.h"

// Sets default values
AScienceQuestExamineOverlap::AScienceQuestExamineOverlap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Sphere
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Overlap"));
	OverlapSphere->SetHiddenInGame(false);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SetRootComponent(OverlapSphere);
}

void AScienceQuestExamineOverlap::DestroySphere(AActor* DestroyedActor)
{
	Destroy();
}

