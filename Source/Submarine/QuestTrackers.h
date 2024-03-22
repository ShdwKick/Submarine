// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Submarine/GameLogic/Structs.h"
#include "UObject/NoExportTypes.h"
#include "QuestTrackers.generated.h"

UENUM(BlueprintType)
enum class EAgent : uint8
{
	ScienceAgent UMETA(DisplayName = "Science Agent"),
	EcologyAgent UMETA(DisplayName = "EcologyAgent"),
	BuisnessAgent UMETA(DisplayName = "BuisnessAgent"),
	GarbageAgent UMETA(DisplayName = "GarbageAgent")
};

//////////////////////////////////////////BASE///////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, EAgent, CompletedQuestAgentType);

// Base tracker
UCLASS()
class SUBMARINE_API UQuestTracker : public UObject
{
	GENERATED_BODY()

public:
	FOnQuestCompleted OnQuestCompleted;
};
 

////////////////////////////////////////GARBAGE//////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGarbageQuestUpdated, int32, GarbageCount);

// Garbage quests tracker
UCLASS()
class SUBMARINE_API UQuestTracker_Garbage : public UQuestTracker
{
	GENERATED_BODY()

public:
	void InitObject(int32 inCountTargetGarbage, class ASubmarinePlayerPawnBase* Player);

	// Delegate for quest update
	FOnGarbageQuestUpdated OnGarbageQuestUpdated;

	// PlayerPassedGarbage
	UFUNCTION()
	void GarbagePassed(int32 GarbagePassed);

	// How many garbage player have at this moment
	int32 CurrentGarbageCount = 0;
	// How many garbage player need to pass to Raccoon
	int32 GarbageCountTarget = 0;

	void SetGarbageCount(int32 NewGarbageCount);
};

////////////////////////////////////////SCIENCE//////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScienceQuestUpdated, float, ExplorationTime);

// Science quests tracker
UCLASS()
class SUBMARINE_API UQuestTracker_Science : public UQuestTracker
{
	GENERATED_BODY()

public:
	void InitObject(TArray<AActor*> inObjectsToInvestigate, float TimeToExamine, float ExplorationRadius);

	//// Delegate for quest update
	FOnScienceQuestUpdated OnScienceQuestUpdated;

protected:

	// Spawned overlap sphere BeginOverlap bing func 
	UFUNCTION()
	void CollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Spawned overlap sphere EndOverlap bing func 
	UFUNCTION()
	void CollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Overlap sphere spawned on target object
	TArray<class AScienceQuestExamineOverlap*> ExamineSpheresOvelap;

	//// Target object
	//AActor* ObjectToInvestigate = nullptr;

	// Time of examining target object
	float ExamineTime = 0.0f;

	// Target time of examining target object
	float ExamineTimeTarget = 0.0f;

	// Examine timer
	FTimerHandle TimerHandle_Examining;

	void ExamineTimerEvent();
};

////////////////////////////////////////ECOLOGY//////////////////////////////////////////////
// Ecology quests tracker
UCLASS()
class SUBMARINE_API UQuestTracker_Ecology : public UQuestTracker
{
	GENERATED_BODY()

public:
	void InitObject(TArray<AActor*> CreaturesToHeal);

	// Creature health updated, check for quest complete
	UFUNCTION()
	void CreatureHealthUpdate(int32 OldState, int32 NewState);
};

////////////////////////////////////////BUISNESS//////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBuisnessQuestUpdated, EItem, ResourseUpdated, int32, NewItemCount, int32, DifferenceValue);

// Buisness quests tracker
UCLASS()
class SUBMARINE_API UQuestTracker_Buisness : public UQuestTracker
{
	GENERATED_BODY()

public:
	void InitObject(TMap<EItem, int32> ResoursesToCollect, class ASubmarinePlayerPawnBase* Player);

	// Delegate for quest update
	FOnBuisnessQuestUpdated OnBuisnessQuestUpdated;

	// Target item count
	TMap<EItem, int32> TargetResoursesToCollect;

	// Player ref
	class ASubmarinePlayerPawnBase* PlayerRef = nullptr;

	// Player Inventory Updated
	UFUNCTION()
	void PlayerInventoryUpdate(EItem ItemType, int32 NewItemCount, int32 DifferenceValue);
};
