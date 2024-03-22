// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InfluenceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBMARINE_API UInfluenceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInfluenceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TMap<TSubclassOf<AActor>, int32> InfluenceValues;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Influence")
	int32 GetMyInfluenceValue(AActor* InfluenceGetter);

	void ComponentInit();
		
};
