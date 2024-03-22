// Fill out your copyright notice in the Description page of Project Settings.


#include "ReputationAndRepQuestsControl.h"
#include "Engine/DataTable.h"
#include "Submarine/GameplayBase/SubmarineGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Submarine/Player/SubmarinePlayerPawnBase.h"

// Initialization of agents
void UReputationAndRepQuestsControl::InitAgents(class ASubmarinePlayerPawnBase* Player)
{
	PlayerRef = Player;
	ASubmarineGameModeBase* GameMode = Cast<ASubmarineGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		// Sience Quests
		ScienceAgent = new RepAgent_Science;
		ScienceAgent->QuestAgentType = EAgent::ScienceAgent;
		ScienceAgent->SetQuestsInfo(GameMode->SienceQuests);
		ScienceAgent->QuestTracker = NewObject<UQuestTracker_Science>(this);
		ScienceAgent->InitializeQuest(this, PlayerRef);
		ScienceAgent->QuestTracker->OnQuestCompleted.AddDynamic(this, &UReputationAndRepQuestsControl::AgentQuestCompleted);
		Cast<UQuestTracker_Science>(ScienceAgent->QuestTracker)->OnScienceQuestUpdated.AddDynamic(this, &UReputationAndRepQuestsControl::ScienceQuestUpdate);

		// Ecology Quests
		EcologyAgent = new RepAgent_Ecology;
		ScienceAgent->QuestAgentType = EAgent::EcologyAgent;
		EcologyAgent->SetQuestsInfo(GameMode->EcologyQuests);
		EcologyAgent->QuestTracker = NewObject<UQuestTracker_Ecology>(this);
		EcologyAgent->InitializeQuest(this, PlayerRef);
		EcologyAgent->QuestTracker->OnQuestCompleted.AddDynamic(this, &UReputationAndRepQuestsControl::AgentQuestCompleted);

		// Buisness Quests
		BuisnessAgent = new RepAgent_Buisness;
		ScienceAgent->QuestAgentType = EAgent::BuisnessAgent;
		BuisnessAgent->SetQuestsInfo(GameMode->BuisnessQuests);
		BuisnessAgent->QuestTracker = NewObject<UQuestTracker_Buisness>(this);
		BuisnessAgent->InitializeQuest(this, PlayerRef);
		BuisnessAgent->QuestTracker->OnQuestCompleted.AddDynamic(this, &UReputationAndRepQuestsControl::AgentQuestCompleted);
		Player->OnItemCountChanged.AddDynamic(Cast<UQuestTracker_Buisness>(BuisnessAgent->QuestTracker), &UQuestTracker_Buisness::PlayerInventoryUpdate);
		Cast<UQuestTracker_Buisness>(BuisnessAgent->QuestTracker)->OnBuisnessQuestUpdated.AddDynamic(this, &UReputationAndRepQuestsControl::BuisnessQuestUpdate);

		// Garbage Quests
		GarbageAgent = new RepAgent_Garbage;
		ScienceAgent->QuestAgentType = EAgent::GarbageAgent;
		GarbageAgent->SetQuestsInfo(GameMode->GarbageQuests);
		GarbageAgent->QuestTracker = NewObject<UQuestTracker_Garbage>(this);
		GarbageAgent->InitializeQuest(this, PlayerRef);
		
		Player->OnGarbagePassed.AddDynamic(Cast<UQuestTracker_Garbage>(GarbageAgent->QuestTracker), &UQuestTracker_Garbage::GarbagePassed);
		GarbageAgent->QuestTracker->OnQuestCompleted.AddDynamic(this, &UReputationAndRepQuestsControl::AgentQuestCompleted);
		Cast<UQuestTracker_Garbage>(GarbageAgent->QuestTracker)->OnGarbageQuestUpdated.AddDynamic(this, &UReputationAndRepQuestsControl::GarbageQuestUpdate);
	}
}

// Get Agent by EAgent enum
QuestAgent* UReputationAndRepQuestsControl::GetAgentByType(EAgent AgentType)
{
	QuestAgent* result = nullptr;

	switch (AgentType)
	{
		case EAgent::ScienceAgent:
			return ScienceAgent;
			break;
		case EAgent::EcologyAgent:
			return EcologyAgent;
			break;
		case EAgent::BuisnessAgent:
			return BuisnessAgent;
			break;
		case EAgent::GarbageAgent:
			return GarbageAgent;
			break;
		default:
			return nullptr;
			break;
	}
}

// Universal quest completed bind func
void UReputationAndRepQuestsControl::AgentQuestCompleted(EAgent AgentType)
{
	OnAgentQuestCompleted.Broadcast(AgentType);
}

void UReputationAndRepQuestsControl::GiveQuestAward(EAgent AgentType)
{
	TMap<ECurrency, int32> Reward = GetAgentByType(AgentType)->CurrentReward;
	PlayerRef->IncreaseCurrency(Reward);
}

void UReputationAndRepQuestsControl::InitializeNewQuest(EAgent AgentType, bool GiveAward, float DelayTime)
{
	if (GiveAward)
	{
		TMap<ECurrency, int32> Reward = GetAgentByType(AgentType)->CurrentReward;
		PlayerRef->IncreaseCurrency(Reward);
	}

	FTimerHandle NewQuestHandle;
	FTimerDelegate NewQuestDel;
	NewQuestDel.BindUFunction(this, FName("NewQuestCallback"), AgentType);
	GetWorld()->GetTimerManager().SetTimer(NewQuestHandle, NewQuestDel, DelayTime, false);
}

void UReputationAndRepQuestsControl::NewQuestCallback(EAgent AgentType)
{
	GetAgentByType(AgentType)->InitializeQuest(this, PlayerRef);
}

////////////////////////////////////////BASE//////////////////////////////////////////////

template<typename T>
T RepAgent_WithIndex::GetNewQuestInfo(UReputationAndRepQuestsControl* Outer)
{
	T result;
	if (DT_QuestsInfo)
	{
		CurrentQuestIndex++;

		if (DT_QuestsInfo)
		{
			// Get Quest Row from table
			TArray<FName> RowNames = DT_QuestsInfo->GetRowNames();
			if (!RowNames.IsEmpty())
			{
				if (RowNames.IsValidIndex(CurrentQuestIndex))
				{
					return *(DT_QuestsInfo->FindRow<T>(RowNames[CurrentQuestIndex], " "));
				}
				else
					Outer->OnAllAgentQuestsCompleted.Broadcast(QuestAgentType);
			}
		}
	}
	return T();
}


////////////////////////////////////////GARBAGE//////////////////////////////////////////////

// Garbage quest updated
void UReputationAndRepQuestsControl::GarbageQuestUpdate(int32 GarbageCount)
{
	OnGarbageQuestUpdatedEvent.Broadcast(GarbageCount);
}

// Garbage new quest initialization
void RepAgent_Garbage::InitializeQuest(UReputationAndRepQuestsControl* Outer, class ASubmarinePlayerPawnBase* Player)
{
	UQuestTracker_Garbage* myTracker = StaticCast<UQuestTracker_Garbage*>(QuestTracker);
	if (DT_QuestsInfo)
	{
		if (myTracker)
		{
			// Get Random Row from table
			TArray<FName> RowNames = DT_QuestsInfo->GetRowNames();
			if (!RowNames.IsEmpty())
			{
				int32 QuestIndex = FMath::RandHelper(RowNames.Num());
				FGarbageQuestsInfo QuestInfo = *(DT_QuestsInfo->FindRow<FGarbageQuestsInfo>(RowNames[QuestIndex], " "));

				// Initialize Quest Tracker
				myTracker->InitObject(QuestInfo.GarbageToCollect, Player);
				CurrentReward.Add(ECurrency::Money, QuestInfo.RewardMoneyAnount);
				CurrentReward.Add(ECurrency::GarbageRep, QuestInfo.RewardReputationAnount);
				Outer->OnNewGarbageQuest.Broadcast(QuestInfo);
			}
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("RepAgent_Garbage::InitializeQuest - QuestTracker -NULL"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("RepAgent_Garbage::InitializeQuest - DT_QuestsTable -NULL"));
}


////////////////////////////////////////SCIENCE//////////////////////////////////////////////

void UReputationAndRepQuestsControl::ScienceQuestUpdate(float ExplorationTime)
{
	OnScienceQuestUpdatedEvent.Broadcast(ExplorationTime);
}

void RepAgent_Science::InitializeQuest(UReputationAndRepQuestsControl* Outer, ASubmarinePlayerPawnBase* Player)
{
	UQuestTracker_Science* myTracker = StaticCast<UQuestTracker_Science*>(QuestTracker);
	FScienceQuestsInfo QuestInfo = GetNewQuestInfo<FScienceQuestsInfo>(Outer);
	CurrentReward.Add(ECurrency::Money, QuestInfo.RewardMoneyAnount);
	CurrentReward.Add(ECurrency::ScienceRep, QuestInfo.RewardReputationAnount);
	if (myTracker)
	{
		TArray<AActor*> ActorsToInvestigate;
		UGameplayStatics::GetAllActorsOfClass(Outer->GetWorld(), QuestInfo.ObjectClassToInvestigate, ActorsToInvestigate);
		myTracker->InitObject(ActorsToInvestigate, QuestInfo.TimeToExamine, QuestInfo.ExplorationRadius);

		Outer->OnNewScienceQuest.Broadcast(QuestInfo);
	}
}


////////////////////////////////////////ECOLOGY//////////////////////////////////////////////

void RepAgent_Ecology::InitializeQuest(UReputationAndRepQuestsControl* Outer, ASubmarinePlayerPawnBase* Player)
{
	UQuestTracker_Ecology* myTracker = StaticCast<UQuestTracker_Ecology*>(QuestTracker);
	FEcologyQuestsInfo QuestInfo = GetNewQuestInfo<FEcologyQuestsInfo>(Outer);
	CurrentReward.Add(ECurrency::Money, QuestInfo.RewardMoneyAnount);
	CurrentReward.Add(ECurrency::EcologyRep, QuestInfo.RewardReputationAnount);
	if (myTracker)
	{
		TArray<AActor*> CreaturesToHeal;
		UGameplayStatics::GetAllActorsOfClass(Outer->GetWorld(), QuestInfo.CreatureClassToHeal, CreaturesToHeal);
		myTracker->InitObject(CreaturesToHeal);
		Outer->OnNewEcologyQuest.Broadcast(QuestInfo);
	}
}



////////////////////////////////////////BUISNESS//////////////////////////////////////////////

void RepAgent_Buisness::InitializeQuest(UReputationAndRepQuestsControl* Outer, ASubmarinePlayerPawnBase* Player)
{
	UQuestTracker_Buisness* myTracker = StaticCast<UQuestTracker_Buisness*>(QuestTracker);
	FBuisnessQuestsInfo QuestInfo = GetNewQuestInfo<FBuisnessQuestsInfo>(Outer);
	CurrentReward.Add(ECurrency::Money, QuestInfo.RewardMoneyAnount);
	CurrentReward.Add(ECurrency::BusinessRep, QuestInfo.RewardReputationAnount);
	if (myTracker)
	{
		myTracker->InitObject(QuestInfo.ResoursesToCollect, Player);
		Outer->OnNewBuisnessQuest.Broadcast(QuestInfo);
	}
}

void UReputationAndRepQuestsControl::BuisnessQuestUpdate(EItem ResourseUpdated, int32 NewItemCount, int32 DifferenceValue)
{
	OnBuisnessQuestUpdatedEvent.Broadcast(ResourseUpdated, NewItemCount, DifferenceValue);
}
