// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Submarine/GameLogic/Structs.h"
#include "Submarine/GameLogic/LivingCreatures/Influence/InfluenceComponent.h"
#include "LivingCreature.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdate, int32, OldState, int32, NewState);

UCLASS()
class SUBMARINE_API ALivingCreature : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALivingCreature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//
	//* COMPONENTS *//
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class USphereComponent* SphereCollision;
	UInfluenceComponent* InfluenceComponent;


	void CreatureInit();

	//
	//* HEALTH LOGIC *//
	//
	// Group health (can be influenced by objects with influence interface)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health = 2.0f;
	int32 CurrentState = 2;
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	FLivingCreatureInfo CreatureInfo;

	FTimerHandle TimerHandle_HealthInfluence;
	float HealthInfluenceTickValue = 0.1f;

	void HealthInfluenceEvent();
	void UpdateHealthState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	int32 CurrentHealthInfluence = 0;

	UFUNCTION(BlueprintNativeEvent)
	void HealthBigChange(int32 OldState, int32 NewState);

	// Health update delegate for quests and widgets
	FOnHealthUpdate OnHealthUpdate;


	////////////////////
	UFUNCTION()
	virtual void SphereCollisionOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void SphereCollisionOnEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
