// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Influence_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInfluence_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUBMARINE_API IInfluence_Interface
{
	GENERATED_BODY()

public:

	virtual int32 GetMyInfluenceValue(AActor* InfluenceGetter) = 0;
};
