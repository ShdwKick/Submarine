// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Trade_Interface.h"
#include "Submarine/GameLogic/Structs.h"
#include "QuestTrackers.h"
#include "ReputationAndRepQuestsControl.generated.h"

USTRUCT(BlueprintType)
struct FRepQuestsInfo : public FTableRowBase
{
	GENERATED_BODY()

	// What type of agent is accociated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Settings")
	EAgent AgentType = EAgent::ScienceAgent;

	// What type of agent is accociated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Settings")
	TArray<int32> QuestValues;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Settings")
	TArray<AActor*> QuestActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Settings")
	FText QuestText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Settings")
	TSubclassOf<UQuestTracker> TrackerType;

};


USTRUCT(BlueprintType)
struct FGarbageQuestsInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText QuestText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 RewardMoneyAnount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 RewardReputationAnount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 GarbageToCollect;
};

USTRUCT(BlueprintType)
struct FScienceQuestsInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText QuestText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText TargetText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 RewardMoneyAnount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 RewardReputationAnount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<AActor> ObjectClassToInvestigate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float TimeToExamine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ExplorationRadius;
};

USTRUCT(BlueprintType)
struct FEcologyQuestsInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText QuestText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 RewardMoneyAnount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 RewardReputationAnount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<AActor> CreatureClassToHeal;
};

USTRUCT(BlueprintType)
struct FBuisnessQuestsInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText QuestText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 RewardMoneyAnount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 RewardReputationAnount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TMap<EItem, int32> ResoursesToCollect;
};

USTRUCT(BlueprintType)
struct FRepGoodsInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Settings")
	EAgent AgentType = EAgent::ScienceAgent;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Base
struct QuestAgent
{
public:
	QuestAgent()
	{}
	virtual ~QuestAgent()
	{}
protected:
	// Quests info
	UDataTable* DT_QuestsInfo = nullptr;

public:
	// Essential info
	EAgent QuestAgentType;

	// Quest tracker uobject
	UQuestTracker* QuestTracker;

	// Current reward
	TMap<ECurrency, int32> CurrentReward;

	virtual void InitializeQuest(UReputationAndRepQuestsControl* Outer, class ASubmarinePlayerPawnBase* Player) = 0;

	// Set Agent Type
	inline void SetAgentType(EAgent newAgentType) { QuestAgentType = newAgentType; };
	// Set Quests info
	inline void SetQuestsInfo(UDataTable* QuestsInfo) { DT_QuestsInfo = QuestsInfo; };
};

// Base with index
struct RepAgent_WithIndex : public QuestAgent
{
protected:
	int32 CurrentQuestIndex = -1;

public:
	template<typename T>
	T GetNewQuestInfo(UReputationAndRepQuestsControl* Outer);
};

/////////////////////////////////////
// Garbage Agent
struct RepAgent_Garbage : public QuestAgent
{
public:
	RepAgent_Garbage()
	{}
	~RepAgent_Garbage()
	{}
	virtual void InitializeQuest(UReputationAndRepQuestsControl* Outer, class ASubmarinePlayerPawnBase* Player) override;
};

/////////////////////////////////////
// Science Agent
struct RepAgent_Science : public RepAgent_WithIndex
{
public:
	RepAgent_Science()
	{}
	~RepAgent_Science()
	{}
	virtual void InitializeQuest(UReputationAndRepQuestsControl* Outer, class ASubmarinePlayerPawnBase* Player) override;
};

/////////////////////////////////////
// Ecology Agent
struct RepAgent_Ecology : public RepAgent_WithIndex
{
public:
	RepAgent_Ecology()
	{}
	~RepAgent_Ecology()
	{}
	virtual void InitializeQuest(UReputationAndRepQuestsControl* Outer, class ASubmarinePlayerPawnBase* Player) override;
};

/////////////////////////////////////
// Buisness Agent
struct RepAgent_Buisness : public RepAgent_WithIndex
{
public:
	RepAgent_Buisness()
	{}
	~RepAgent_Buisness()
	{}
	virtual void InitializeQuest(UReputationAndRepQuestsControl* Outer, class ASubmarinePlayerPawnBase* Player) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewGarbageQuest, FGarbageQuestsInfo, QuestInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGarbageQuestUpdatedEvent, int32, GarbageCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewScienceQuest, FScienceQuestsInfo, QuestInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScienceQuestUpdatedEvent, float, ExplorationTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewBuisnessQuest, FBuisnessQuestsInfo, QuestInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBuisnessQuestUpdatedEvent, EItem, ResourseUpdated, int32, NewItemCount, int32, DifferenceValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewEcologyQuest, FEcologyQuestsInfo, QuestInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAgentQuestCompleted, EAgent, AgentType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllAgentQuestsCompleted, EAgent, AgentType);


UCLASS(Blueprintable, BlueprintType)
class SUBMARINE_API UReputationAndRepQuestsControl : public UObject
{
	GENERATED_BODY()

public:
	UReputationAndRepQuestsControl()
	{}
	~UReputationAndRepQuestsControl()
	{
		delete ScienceAgent;
		delete EcologyAgent;
		delete BuisnessAgent;
		delete GarbageAgent;

		ScienceAgent = nullptr;
		EcologyAgent = nullptr;
		BuisnessAgent = nullptr;
		GarbageAgent = nullptr;
	}

	RepAgent_Science* ScienceAgent = nullptr;
	RepAgent_Ecology* EcologyAgent =  nullptr;
	RepAgent_Buisness* BuisnessAgent = nullptr;
	RepAgent_Garbage* GarbageAgent = nullptr;

	class ASubmarinePlayerPawnBase* PlayerRef;

	void InitAgents(class ASubmarinePlayerPawnBase* Player);

	QuestAgent* GetAgentByType(EAgent AgentType);

	// Quest is complete, go to next quest
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void AgentQuestCompleted(EAgent AgentType);

	// Quest is complete, go to next quest
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void GiveQuestAward(EAgent AgentType);


	// All agents quests completed delegate
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnAllAgentQuestsCompleted OnAllAgentQuestsCompleted;

	// Quest Completed Event
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnAgentQuestCompleted OnAgentQuestCompleted;

	// Initialize new quest for agent
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void InitializeNewQuest(EAgent AgentType, bool GiveAward = true, float DelayTime = 20.f);

	UFUNCTION()
	void NewQuestCallback(EAgent AgentType);

	// Quests Updates
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnGarbageQuestUpdatedEvent OnGarbageQuestUpdatedEvent;
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnScienceQuestUpdatedEvent OnScienceQuestUpdatedEvent;
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnBuisnessQuestUpdatedEvent OnBuisnessQuestUpdatedEvent;

	// New quests events
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnNewGarbageQuest OnNewGarbageQuest;
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnNewScienceQuest OnNewScienceQuest;
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnNewEcologyQuest OnNewEcologyQuest;
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnNewBuisnessQuest OnNewBuisnessQuest;

	UFUNCTION()
	void GarbageQuestUpdate(int32 GarbageCount);
	UFUNCTION()
	void ScienceQuestUpdate(float ExplorationTime);
	UFUNCTION()
	void BuisnessQuestUpdate(EItem ResourseUpdated, int32 NewItemCount, int32 DifferenceValue);

	//UPROPERTY(BlueprintAssignable, Category = "Quests")
	//FOnScienceQuestUpdatedEvent OnScienceQuestUpdatedEvent;

	//UPROPERTY(BlueprintAssignable, Category = "Quests")
	//FOnBuisnessQuestUpdatedEvent OnBuisnessQuestUpdatedEvent;
};
