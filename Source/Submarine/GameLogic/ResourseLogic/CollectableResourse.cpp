// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableResourse.h"
#include "Submarine/Player/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include <Sound/SoundWave.h>
#include "SoundDefinitions.h"

// Sets default values
ACollectableResourse::ACollectableResourse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a mesh...
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));

	//Create a sphere...
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->AttachToComponent(MyMesh, FAttachmentTransformRules::KeepRelativeTransform);
	Sphere->SetRelativeLocation(FVector(0.f, 0.f, 28.f));
	Sphere->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	Sphere->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block); // to block trace for interactables

	//Create particle system...
	Shine = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shine"));
	Shine->SetupAttachment(MyMesh);
}

// Called when the game starts or when spawned
void ACollectableResourse::BeginPlay()
{
	Super::BeginPlay();

	// Set stats from data table
	MyData = ResourseDataTable->FindRow<FResourseData>(MyName, FString(""));

	// Set everything for overlap events
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACollectableResourse::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACollectableResourse::OnOverlapEnd);
	if (IsOverlappingActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		MyMesh->SetOverlayMaterial(OverlayMaterial);
		Shine->Activate();
	}
	
}

// Called every frame
void ACollectableResourse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectableResourse::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		MyMesh->SetOverlayMaterial(OverlayMaterial);
		Shine->Activate();
	}
}

void ACollectableResourse::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		MyMesh->SetOverlayMaterial(nullptr);
		Shine->Deactivate();
	}
}

void ACollectableResourse::Interact(AActor* Interactor, bool bShort)
{
	UGameplayStatics::PlaySound2D(GetWorld(), CollectResourseSound);
	IPlayerInterface* Player = Cast<IPlayerInterface>(Interactor);
	if (Player)
	{
		Player->CollectedResourse(MyData->ItemType, 1, MyData->EnergyConsumption);
	}
		Destroy();
}

bool ACollectableResourse::InteractConditions(AActor* Interactor)
{
	IPlayerInterface* Player = Cast<IPlayerInterface>(Interactor);
	bool OverlapPlayer = IsOverlappingActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	return((Player->GetCurrentEnergy() >= MyData->EnergyConsumption) && (OverlapPlayer));
}

void ACollectableResourse::InteractionRefused(AActor* Interactor)
{
	IPlayerInterface* Player = Cast<IPlayerInterface>(Interactor);
	if (IsOverlappingActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		Player->ShowNotification(FText::FromString(TEXT("Не хватает энергии!")));
	else
		Player->ShowNotification(FText::FromString(TEXT("Подойдите ближе!")));
}

