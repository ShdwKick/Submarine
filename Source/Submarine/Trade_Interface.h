// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Trade_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTrade_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUBMARINE_API ITrade_Interface
{
	GENERATED_BODY()

public:

	virtual bool Buy(AActor* Instigator) = 0;
	virtual bool Sell(AActor* Instigator) = 0;

};
