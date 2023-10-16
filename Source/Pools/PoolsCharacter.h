// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPool.h"
#include "RPG/Status.h"
#include "PoolsCharacter.generated.h"

UCLASS(Blueprintable)
class APoolsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APoolsCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// resizeable array of statuses currently affecting us
	TPool<FStatusInstance, 16> Statuses;

	// array of statuses to assign at random
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UStatus*> StatusLibrary;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStatusPoolUI* StatusPoolUI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Rate;
	
	FStatusInstance* Item1Buff;
	FStatusInstance* Item2Buff;

	UFUNCTION()
	void EquipItem1() { EquipItem(Item1Buff);}
	UFUNCTION()
	void EquipItem2() { EquipItem(Item2Buff);}

	void EquipItem(FStatusInstance*& inst);
	
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

