// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUBMARINE_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Interact event. Short - press, not Short (long) - hold
	virtual void Interact(AActor* Interactor, bool bShort);

	// End of the lond interaction
	virtual void InteractEnd(AActor* Interactor);

	// Check if player can interact with the object
	virtual bool InteractConditions(AActor* Interactor);

	// Interact conditions are not met
	virtual void InteractionRefused(AActor* Interactor);
};
