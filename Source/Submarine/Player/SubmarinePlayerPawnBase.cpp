// Fill out your copyright notice in the Description page of Project Settings.


#include "SubmarinePlayerPawnBase.h"
#include "Submarine/GameLogic/InteractionBase/InteractInterface.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ASubmarinePlayerPawnBase::ASubmarinePlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a capsule...
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->SetupAttachment(RootComponent);
	CollisionCapsule->InitCapsuleSize(125.f, 125.f);
	CollisionCapsule->SetCollisionProfileName(FName("Pawn"), true);
	CollisionCapsule->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap);
	CollisionCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	//// Create a camera boom...
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(CollisionCapsule);
	//CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when pawn does
	//CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Camera
	CameraAnglePitch = 0;
	CameraBoomAnglePitch = -60;
}

// Called when the game starts or when spawned
void ASubmarinePlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame. Debug messages
void ASubmarinePlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASubmarinePlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Get the player controller
	APlayerController* PC = Cast<APlayerController>(GetController());

	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	// Clear out existing mapping, and add our mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind the actions
	PEI->BindAction(InteractAction, ETriggerEvent::Started, this, &ASubmarinePlayerPawnBase::InteractInput);
	PEI->BindAction(InteractAction, ETriggerEvent::Completed, this, &ASubmarinePlayerPawnBase::EndPressInteract);

}

void ASubmarinePlayerPawnBase::InteractInput()
{

	APlayerController* PC = Cast<APlayerController>(GetController());

	// Prevent interaction blocked by touching move stick
	bool IsTouch2Presed;
	double X;
	double Y;
	PC->GetInputTouchState(ETouchIndex::Touch2, X, Y, IsTouch2Presed);

	// Get HitResult under finger
	if (IsTouch2Presed)
		PC->GetHitResultUnderFingerByChannel(ETouchIndex::Touch2, ETraceTypeQuery::TraceTypeQuery4, false, HitResult);
	else
		PC->GetHitResultUnderFingerByChannel(ETouchIndex::Touch1, ETraceTypeQuery::TraceTypeQuery4, false, HitResult);

	// for PC
	//PC->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery4, false, HitResult); 

	if (HitResult.bBlockingHit)
	{
		GetWorldTimerManager().SetTimer(ShortPressTimer, this, &ASubmarinePlayerPawnBase::LongPressInteract,  ShortPressTime, false);
	}
}

void ASubmarinePlayerPawnBase::ResourseCollected_Implementation(EItem ItemType)
{
}

void ASubmarinePlayerPawnBase::Notification_Implementation(const FText& NotifText)
{
}

void ASubmarinePlayerPawnBase::AddResourse(EItem ItemType, int32 Count)
{
	int32 ResourseCount = 0;

	// Add resourse
	if (Inventory.Contains(ItemType))
	{
		ResourseCount = *(Inventory.Find(ItemType));
		Inventory.Add(ItemType, ResourseCount + Count);
	}
	else
		Inventory.Add(ItemType, Count);

	OnItemCountChanged.Broadcast(ItemType, ResourseCount + Count, Count);
}

void ASubmarinePlayerPawnBase::ShowNotification(FText NotificationText)
{
	//Notification(NotificationText);
}

void ASubmarinePlayerPawnBase::CollectedResourse(EItem ItemType, int32 Count, float EnergyConsumption)
{

	// Consume energy
	PlayerEnergy -= EnergyConsumption;
	//GetWorldTimerManager().UnPauseTimer(EnergyRestorationTimer);

	AddResourse(ItemType, Count);
	ResourseCollected(ItemType);
}

bool ASubmarinePlayerPawnBase::TakeAwayResourse(EItem ItemType, int32 Count)
{
	bool result = false;
	if (Inventory.Contains(ItemType))
	{
		int32 CurrentCount = *Inventory.Find(ItemType);
		if (CurrentCount >= Count)
		{
			Inventory.Add(ItemType, CurrentCount - Count);
			OnItemCountChanged.Broadcast(ItemType, CurrentCount - Count, Count);
			result = true;
		}
	}

	return result;
}

bool ASubmarinePlayerPawnBase::TakeAwayResourses(TMap<EItem, int32> Resourses)
{
	bool result = true;

	if (CheckInventoryHaveResourses(Resourses))
	{
		TArray<EItem> Keys;
		Resourses.GetKeys(Keys);
		for (auto elem : Keys)
		{
			if (Inventory.Contains(elem))
			{
				int32 ResourseRemain = *Inventory.Find(elem);
				ResourseRemain -= *Resourses.Find(elem);
				Inventory.Add(elem, ResourseRemain);

				OnItemCountChanged.Broadcast(elem, ResourseRemain, *Inventory.Find(elem));
			}
		}
	}
	else
		result = false;

	return result;
}

float ASubmarinePlayerPawnBase::GetCurrentEnergy()
{
	return PlayerEnergy;
}

bool ASubmarinePlayerPawnBase::CheckIfPlayerHaveCurrency(TMap<ECurrency, int32> Cost)
{
	bool result = true;

	TArray<ECurrency> Keys;
	Cost.GetKeys(Keys);
	if (!Keys.IsEmpty())
	{
		for (ECurrency elem : Keys)
		{
			if (Wallet.Contains(elem))
			{
				if (*Wallet.Find(elem) < *Cost.Find(elem))
				{
					// Wallet dont have enough one of currency
					result = false;
					break;
				}
			}
			else
			{
				// Wallet dont contains one of currency
				result = false;
				break;
			}
		}
	}
	else
	{
		// Wallet is empty
		result = false;
	}

	return result;
}

bool ASubmarinePlayerPawnBase::PayCurrency(TMap<ECurrency, int32> Cost)
{
	bool result = true;

	if (CheckIfPlayerHaveCurrency(Cost))
	{
		TArray<ECurrency> Keys;
		Cost.GetKeys(Keys);
		for (auto elem : Keys)
		{
			if (Wallet.Contains(elem))
			{
				int32 CurrencyRemain = *Wallet.Find(elem);
				CurrencyRemain -= *Cost.Find(elem);
				Wallet.Add(elem, CurrencyRemain);

				OnCurrencyCountChanged.Broadcast(elem, CurrencyRemain, false, *Cost.Find(elem));
			}
		}
	}
	else
		result = false;

	return result;
}

void ASubmarinePlayerPawnBase::IncreaseCurrency(TMap<ECurrency, int32> Earnings)
{
	TArray<ECurrency> Keys;
	Earnings.GetKeys(Keys);
	if (!Keys.IsEmpty())
	{
		for (auto elem : Keys)
		{
			int32 CurrencyCount = 0;
			if (Wallet.Contains(elem))
				CurrencyCount = *Wallet.Find(elem);

			CurrencyCount += *Earnings.Find(elem);
			Wallet.Add(elem, CurrencyCount);

			OnCurrencyCountChanged.Broadcast(elem, CurrencyCount, true, *Earnings.Find(elem));
		}
	}
}

void ASubmarinePlayerPawnBase::SwitchRegenerateEnergy(bool Enable)
{
	FTimerManager& timerManager = GetWorldTimerManager();
	if (Enable)
	{
		timerManager.SetTimer(EnergyRestorationTimer, this,&ASubmarinePlayerPawnBase::IncreasePlayerEnergy,EnergyIncreaseRate, true);
	}
	else
	{
		if(timerManager.IsTimerActive(EnergyRestorationTimer))
		{
			timerManager.PauseTimer(EnergyRestorationTimer);
			timerManager.ClearTimer(EnergyRestorationTimer);
		}
	}
}

void ASubmarinePlayerPawnBase::IncreasePlayerEnergy()
{
	if(PlayerEnergy < MaxEnergyValue)
	{
		PlayerEnergy += EnergyIncreaseValue;
	}
}

bool ASubmarinePlayerPawnBase::CheckInventoryHaveResourses(TMap<EItem, int32> ResoursesToCheck)
{
	bool result = true;
	TArray<EItem> ItemArray;
	ResoursesToCheck.GetKeys(ItemArray);
	if (!ItemArray.IsEmpty())
	{
		for (EItem elem : ItemArray)
		{
			if (Inventory.Contains(elem))
			{
				int32 ResourseNeeded = *ResoursesToCheck.Find(elem);
				int32 ResourseHave = *Inventory.Find(elem);

				if (ResourseHave < ResourseNeeded)
				{

					// Player don't have enough of one resourse
					result = false;
					//break;
				}
			}
			else
			{
				// Player don't have one resourse
				result = false;
				//break;
			}
		}
	}
	else
	{
		// ResoursesToCheck map is empty
		result = false;
	}

	return result;
}

bool ASubmarinePlayerPawnBase::CanTakeMoreGarbage()
{
	return GarbageCollected < MaxGarbage;
}

void ASubmarinePlayerPawnBase::CollectGarbage()
{
	GarbageCollected++;
	OnGarbageCollected.Broadcast(GarbageCollected);
}

int32 ASubmarinePlayerPawnBase::PassGarbage()
{
	int32 GarbageOut = GarbageCollected;
	GarbageCollected = 0;
	OnGarbagePassed.Broadcast(GarbageOut);
	return GarbageOut;
}

bool ASubmarinePlayerPawnBase::CanFeedCratures()
{
	if (Inventory.Contains(EItem::Food))
	{
		if (*Inventory.Find(EItem::Food) > 0)
			return true;
	}

	return false;
}

void ASubmarinePlayerPawnBase::RemoveOneFood()
{
	if (Inventory.Contains(EItem::Food))
	{
		int32 FoodCount = *Inventory.Find(EItem::Food);
		Inventory.Add(EItem::Food, FoodCount-1);
		OnItemCountChanged.Broadcast(EItem::Food, FoodCount - 1, -1);
	}
}

int32 ASubmarinePlayerPawnBase::GetCurrentFood()
{
	int32 FoodCount = 0;
	if (Inventory.Find(EItem::Food))
		FoodCount = *(Inventory.Find(EItem::Food));
	return FoodCount;
}

void ASubmarinePlayerPawnBase::BaseOverlapChanged(bool IsOverlapping)
{
	bIsAtBase = IsOverlapping;
	SwitchRegenerateEnergy(IsOverlapping);
	IsOnBaseChanged.Broadcast(IsOverlapping);
}


void ASubmarinePlayerPawnBase::LongPressInteract()
{
	IInteractInterface* ActorToInteract = Cast<IInteractInterface>(HitResult.GetActor());
	if (ActorToInteract)
	{
		//check for interact conditions
		if (ActorToInteract->InteractConditions(this))
		{
			ActorToInteract->Interact(this, false); // long (hold)
		}
		else
		{
			ActorToInteract->InteractionRefused(this);
		}
	}
	GetWorldTimerManager().ClearTimer(ShortPressTimer);

}

void ASubmarinePlayerPawnBase::EndPressInteract()
{
	// check if shortPressTime is over
	if (GetWorldTimerManager().IsTimerActive(ShortPressTimer))
	{
		IInteractInterface* ActorToInteract = Cast<IInteractInterface>(HitResult.GetActor());
		if (ActorToInteract)
		{
			//check for interact conditions
			if (ActorToInteract->InteractConditions(this))
			{
				ActorToInteract->Interact(this, true); // short (press)
			}
			else
			{
				ActorToInteract->InteractionRefused(this);
			}
		}		
		GetWorldTimerManager().ClearTimer(ShortPressTimer);
	}
	else
	{
		IInteractInterface* ActorToInteract = Cast<IInteractInterface>(HitResult.GetActor());
		if (ActorToInteract)
		{
			ActorToInteract->InteractEnd(this); // "long (hold) interact" was already called, calling "end pressing"
		}
	}
}


