// Fill out your copyright notice in the Description page of Project Settings.

#include "SubmarinePlayerControllerBase.h"
#include "Submarine/ReputationAndRepQuestsControl.h"
#include "Blueprint/UserWidget.h"
#include "Submarine/Player/SubmarinePlayerPawnBase.h"

void ASubmarinePlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	// Create quests control
	QuestsControlObject = NewObject<UReputationAndRepQuestsControl>(this);

	// Create HUD, it binds to QuestsControl Events
	HUD = CreateWidget(this, InGameWidget);
	HUD->AddToViewport(9999);

	// Initiate quests in Quests Control
	ASubmarinePlayerPawnBase* myPlayer = Cast<ASubmarinePlayerPawnBase>(GetPawn());
	QuestsControlObject->InitAgents(myPlayer);
}