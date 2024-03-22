// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerInterface.h"
#include "Components/CapsuleComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Submarine/GameLogic/Structs.h"
#include "SubmarinePlayerPawnBase.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemCountChanged, EItem, ItemType, int32, NewItemCount, int32, DifferenceValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCurrencyCountChanged, ECurrency, CurrencyType, int32, NewCurrencyCount, bool, Add, int32, DifferenceValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsOnBaseChanged, bool, IsOnBase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGarbageCollected, int32, CurrentGarbageCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGarbagePassed, int32, GarbagePassed);


UCLASS()
class SUBMARINE_API ASubmarinePlayerPawnBase : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASubmarinePlayerPawnBase();

/////////////////////////////////// Delegates /////////////////////////////////////
// 
	// Delegate "on item count change" for widget
	UPROPERTY(BlueprintAssignable)
	FOnItemCountChanged OnItemCountChanged;

	// Delegate "on currency count change" for widget
	UPROPERTY(BlueprintAssignable)
	FOnCurrencyCountChanged OnCurrencyCountChanged;

	// Delegate "is on base change" for widget
	UPROPERTY(BlueprintAssignable)
	FIsOnBaseChanged IsOnBaseChanged;

	// Delegte for HUD widget
	UPROPERTY(BlueprintAssignable)
	FOnGarbageCollected OnGarbageCollected;

	// Delegte for HUD widget and garbage quest tracker
	UPROPERTY(BlueprintAssignable)
	FOnGarbagePassed OnGarbagePassed;

/////////////////////////////////////////////////////////////////////////////////

	///** Returns CameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return CameraComponent; }
	///** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	///** Camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* CameraComponent;

	///** Camera boom positioning the camera above the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;

	/** Capsule for collisions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionCapsule;



public:
	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* DefaultMappingContext;
	
	/** Interact Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InteractAction;
	FTimerHandle ShortPressTimer;
	void LongPressInteract();
	void EndPressInteract();
	FHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShortPressTime = 0.1;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InteractInput();

	/** Energy */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Energy")
		float PlayerEnergy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Energy")
		float MaxEnergyValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Energy")
		float EnergyIncreaseValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Energy")
		float EnergyIncreaseRate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Energy")
		FTimerHandle EnergyRestorationTimer;
	void SwitchRegenerateEnergy(bool Enable);
	void IncreasePlayerEnergy();

	/** Inventory */

	// Old. Still have references, so cant be deleted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<TSubclassOf<AActor>, int32> PlayerInventory;

	// New inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<EItem, int32> Inventory;

	// Wallet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<ECurrency, int32> Wallet;

	// Called in blueprint to play animation and call AC_PlayerInventory's event dispatcher
	UFUNCTION(BlueprintNativeEvent)
	void ResourseCollected(EItem ItemType);

	// For showing notifications. Logic is on blueprints
	UFUNCTION(BlueprintNativeEvent)
	void Notification(const FText& NotifText);

	/**  Camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraAnglePitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraBoomAnglePitch;

	// Resourse bought
	UFUNCTION(BlueprintCallable, Category = "Resourses")
	void AddResourse(EItem ItemType, int32 Count);


	// From PlayerInterface

	void ShowNotification(FText NotificationText) override;
	UFUNCTION(BlueprintCallable, Category = "Resourses")
	void CollectedResourse(EItem ItemType, int32 Count, float EnergyConsumption) override;
	UFUNCTION(BlueprintCallable, Category = "Resourses")
	virtual bool TakeAwayResourse(EItem ItemType, int32 Count) override;
	UFUNCTION(BlueprintCallable, Category = "Resourses")
	virtual bool TakeAwayResourses(TMap<EItem, int32> Resourses) override;
	UFUNCTION(BlueprintCallable, Category = "Resourses")
	bool CheckInventoryHaveResourses(TMap<EItem, int32> ResoursesToCheck) override;
	float GetCurrentEnergy() override;

	// Trade from PlayerInterface
	UFUNCTION(BlueprintCallable, Category = "Wallet")
	virtual bool CheckIfPlayerHaveCurrency(TMap<ECurrency, int32> Cost) override;
	UFUNCTION(BlueprintCallable, Category = "Wallet")
	virtual bool PayCurrency(TMap<ECurrency, int32> Cost) override;
	UFUNCTION(BlueprintCallable, Category = "Wallet")
	virtual void IncreaseCurrency(TMap<ECurrency, int32> Earnings) override;

	/////////////NEW MECHANICS///////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bDashing = false;

	// Garbage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Garbage")
	int32 GarbageCollected = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Garbage")
	int32 MaxGarbage = 8;
	bool CanTakeMoreGarbage() override;
	void CollectGarbage() override;

	UFUNCTION(BlueprintCallable, Category = "Garbage")
	int32 PassGarbage();

	// Feeding creatures
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanFeedCratures();
	UFUNCTION(BlueprintCallable)
	void RemoveOneFood();
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentFood();

	// Base
	bool bIsAtBase = false;

	// From PlayerInterface. Player base overlap changed
	virtual void BaseOverlapChanged(bool IsOverlapping) override;

};
