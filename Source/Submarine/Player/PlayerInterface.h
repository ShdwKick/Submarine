// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Submarine/GameLogic/Structs.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUBMARINE_API IPlayerInterface
{
	GENERATED_BODY()

public:
	// Show notification in HUD
	virtual void ShowNotification(FText NotificationText) = 0;

	/* Resourses */
	// Add resourse to inventory
	virtual void CollectedResourse(EItem ItemType, int32 Count, float EnergyConsumption) PURE_VIRTUAL(IPlayerInterface::CollectedResourse, );
	// Take resourse from inventory
	virtual bool TakeAwayResourse(EItem ItemType, int32 Count) PURE_VIRTUAL(IPlayerInterface::TakeAwayResourse, return false;);
	// Take resourse from inventory
	virtual bool TakeAwayResourses(TMap<EItem, int32> Resourses) PURE_VIRTUAL(IPlayerInterface::TakeAwayResourses, return false;);
	// Check for map of items in inventory
	virtual bool CheckInventoryHaveResourses(TMap<EItem, int32> ResoursesToCheck) PURE_VIRTUAL(IPlayerInterface::CheckInventoryHaveResourses, return false;);

	// Get current player energy
	virtual float GetCurrentEnergy() PURE_VIRTUAL(IPlayerInterface::GetCurrentEnergy, return 0.f;);

	/* Garbage */
	// Check if there is space for more garbage to collect
	virtual bool CanTakeMoreGarbage() PURE_VIRTUAL(IPlayerInterface::CanTakeMoreGarbage, return false;);
	// Collect garbage
	virtual void CollectGarbage() PURE_VIRTUAL(IPlayerInterface::CollectGarbage, );

	// Base overlapped
	virtual void BaseOverlapChanged(bool IsOverlapping) PURE_VIRTUAL(IPlayerInterface::BaseOverlapChanged, );

	/* Trade */
	// Check for currency in player's wallet
	virtual bool CheckIfPlayerHaveCurrency(TMap<ECurrency, int32> Cost) PURE_VIRTUAL(IPlayerInterface::CheckIfPlayerHaveCurrency, return false;);
	// Get curtain amount of currency from player's wallet
	virtual bool PayCurrency(TMap<ECurrency, int32> Cost) PURE_VIRTUAL(IPlayerInterface::PayCurrency, return false;);
	// Increase currency in player's wallet
	virtual void IncreaseCurrency(TMap<ECurrency, int32> Earnings) PURE_VIRTUAL(IPlayerInterface::TakeCurrency, );
};
