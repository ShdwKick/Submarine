// Fill out your copyright notice in the Description page of Project Settings.


#include "InfluenceComponent.h"
#include "Submarine/GameplayBase/SubmarineGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInfluenceComponent::UInfluenceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInfluenceComponent::BeginPlay()
{
	Super::BeginPlay();

	ComponentInit();
	
}


// Called every frame
void UInfluenceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UInfluenceComponent::GetMyInfluenceValue(AActor* InfluenceGetter)
{
	int32 result = 0;
	if (InfluenceValues.Contains(InfluenceGetter->GetClass()))
	{
		result = *InfluenceValues.Find(InfluenceGetter->GetClass());
	}
	else
		UE_LOG(LogTemp, Error, TEXT("UInfluenceComponent::GetMyInfluenceValue - no data in InfluenceTable about InfluenceGetter"));

	return result;
}

void UInfluenceComponent::ComponentInit()
{
	USubmarineGameInstance* myGI = Cast<USubmarineGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (myGI)
	{
		FInfluenceInfo myInfo;
		if (myGI->GetInfluenceInfoByClass(GetOwner()->GetClass(), myInfo))
		{
			InfluenceValues = myInfo.Influence;
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("UInfluenceComponent::ComponentInit - GameInstance class is not correct"));
}

