// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractInterface.h"

// Add default functionality here for any IInteractInterface functions that are not pure virtual.

void IInteractInterface::Interact(AActor* Interactor, bool bShort)
{
}

void IInteractInterface::InteractEnd(AActor* Interactor)
{
}

bool IInteractInterface::InteractConditions(AActor* Interactor)
{
	return true;
}

void IInteractInterface::InteractionRefused(AActor* Interactor)
{
}
