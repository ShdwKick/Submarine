// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Submarine/Player/PlayerInterface.h"
#include "Components/DecalComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AMiniBase::AMiniBase()
{

	// Creating MeshComponent...
	MiniBaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiniBase Mesh"));
	MiniBaseMeshComponent->SetupAttachment(RootComponent);

	// Creating interact sphere...
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Sphere"));
	InteractSphere->SetupAttachment(RootComponent);

	// Creating decal component...
	BuildDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Build Decal"));
	BuildDecal->SetupAttachment(RootComponent);

	if (bConstructed)
	{
		BuildDone();
	}
}

void AMiniBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FindedBases;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMiniBase::StaticClass(), FindedBases);
	int32 MinibaseCount = 0;
	if (!FindedBases.IsEmpty())
	{
		for (auto elem : FindedBases)
		{
			AMiniBase* MiniBaseElem = Cast<AMiniBase>(elem);
			if (MiniBaseElem)
			{
				if (MiniBaseElem->bConstructed)
				{
					MinibaseCount++;
				}
			}
		}
	}

	if (BuildingCostTable)
	{
		FMinibaseBuildingInfo* BuildingInfo = BuildingCostTable->FindRow<FMinibaseBuildingInfo>(FName(FString::FromInt(MinibaseCount + 1)), FString(""));
		if (BuildingInfo)
		{
			BuildCost = BuildingInfo->BuildCost;
			UpdateWidget(false);
		}
	}
}

bool AMiniBase::CheckIsBaseEnabled()
{
	return bConstructed;
}

void AMiniBase::Interact(AActor* Interactor, bool bShort)
{
	if (!bShort)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_BuldingTimer, this, &AMiniBase::BuildDone, BuildingTime);
		BuildingSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BuildingSound, GetActorLocation());
	}
}

void AMiniBase::InteractEnd(AActor* Interactor)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BuldingTimer);
	if (BuildingSoundComponent)
	{
		BuildingSoundComponent->Stop();
		BuildingSoundComponent = nullptr;
	}
}

bool AMiniBase::InteractConditions(AActor* Interactor)
{
	bool result = false;
	IPlayerInterface* MyInterface = Cast<IPlayerInterface>(Interactor);
	if (MyInterface)
	{
		if (MyInterface->CheckInventoryHaveResourses(BuildCost))
		{
			result = true;
		}
		else
			MyInterface->ShowNotification(FText::FromString(TEXT("Не хватает ресурсов для постройки мини-базы!")));

		RememberedPlayer = MyInterface;
	}
	return result;
}

void AMiniBase::InteractionRefused(AActor* Interactor)
{
}

void AMiniBase::BuildDone()
{
	if (RememberedPlayer)
	{
		if (RememberedPlayer->TakeAwayResourses(BuildCost))
		{
			// Set MiniBase mesh
			MiniBaseMeshComponent->SetStaticMesh(MiniBaseMesh);
			BuildDecal->DestroyComponent();
			InteractSphere->DestroyComponent();

			// Now base functions will enable
			bConstructed = true;

			TArray<AActor*> FindedBases;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMiniBase::StaticClass(), FindedBases);
			if (!FindedBases.IsEmpty())
			{
				for (auto elem : FindedBases)
				{
					AMiniBase* MiniBaseElem = Cast<AMiniBase>(elem);
					if (MiniBaseElem)
					{
						MiniBaseElem->UpdateCost();
					}
				}
			}

			UpdateWidget(true);

			// If player overlapped, show widget etc
			CheckIsPlayerOverlapped();
		}
	}
}

void AMiniBase::UpdateCost()
{
	if (BuildingCostTable)
	{
		TArray<AActor*> FindedBases;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMiniBase::StaticClass(), FindedBases);
		int32 MinibaseCount = 0;
		if (!FindedBases.IsEmpty())
		{
			for (auto elem : FindedBases)
			{
				AMiniBase* MiniBaseElem = Cast<AMiniBase>(elem);
				if (MiniBaseElem && MiniBaseElem->bConstructed)
				{
					MinibaseCount++;
				}
			}
		}
		FMinibaseBuildingInfo* BuildingInfo = BuildingCostTable->FindRow<FMinibaseBuildingInfo>(FName(FString::FromInt(MinibaseCount + 1)), FString(""));
		if (BuildingInfo)
		{
			BuildCost = BuildingInfo->BuildCost;
			UpdateWidget(false);
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("AMiniBase::BuildDone - BuildingCostTable -NULL"));
}

void AMiniBase::UpdateWidget_Implementation(bool bHide)
{
}
