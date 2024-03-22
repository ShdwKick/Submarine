// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base.h"
#include "Submarine/GameLogic/InteractionBase/InteractInterface.h"
#include "Submarine/GameLogic/Structs.h"
#include "MiniBase.generated.h"

/**
 * 
 */
UCLASS()
class SUBMARINE_API AMiniBase : public ABase, public IInteractInterface
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AMiniBase();

private:
	FTimerHandle TimerHandle_BuldingTimer;

	void BeginPlay() override;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class USphereComponent* InteractSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class UStaticMeshComponent* MiniBaseMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class UDecalComponent* BuildDecal = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UStaticMesh* MiniBaseMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float BuildingTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	class USoundBase* BuildingSound = nullptr;

	class UAudioComponent* BuildingSoundComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	TMap<EItem, int32> BuildCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	UDataTable* BuildingCostTable;

	// Determine if base should construct at loading. For debug and saving
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConstructed = false;

	// Check if base should regenerate player;s energy and enable base's widget
	virtual bool CheckIsBaseEnabled() override;

	// Interact event. Short - press, not Short (long) - hold
	virtual void Interact(AActor* Interactor, bool bShort) override;

	// End of the lond interaction
	virtual void InteractEnd(AActor* Interactor) override;

	// Check if player can interact with the object
	virtual bool InteractConditions(AActor* Interactor) override;

	// Interact conditions are not met
	virtual void InteractionRefused(AActor* Interactor) override;

	void BuildDone();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateWidget(bool bHide);

	void UpdateCost();

	class IPlayerInterface* RememberedPlayer = nullptr;
};
