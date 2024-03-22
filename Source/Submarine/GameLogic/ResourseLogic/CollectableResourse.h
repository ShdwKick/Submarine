// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Submarine/GameLogic/InteractionBase/InteractInterface.h"
#include "Engine/DataTable.h"
#include "Submarine/GameLogic/Structs.h"
#include "CollectableResourse.generated.h"

UCLASS()
class SUBMARINE_API ACollectableResourse : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableResourse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UDataTable* ResourseDataTable;

	FResourseData* MyData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Stats
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetEnergyConsumption() { return MyData->EnergyConsumption; }

	// For override in blueprints
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USoundBase* CollectResourseSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMaterial* OverlayMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* MyMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USphereComponent* Sphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UParticleSystemComponent* Shine;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName MyName;

	// Overlap evnt
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);


	//From interface 
	void Interact(AActor* Interactor, bool bShort) override;
	bool InteractConditions(AActor* Interactor) override;
	void InteractionRefused(AActor* Interactor) override;
};
