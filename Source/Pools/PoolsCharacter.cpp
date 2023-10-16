// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoolsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "RPG/StatusPoolUI.h"

APoolsCharacter::APoolsCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Item1Buff = nullptr;
	Item2Buff = nullptr;
}

void APoolsCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// loop over all valid entries and count them down (if not permanent with timer = -1)
	// if they expire, set the Status pointer to null to invalid that entry and allow for recycling 
	int count = 0;
	for (auto it = Statuses.Begin(); it.IsValid(); ++it)
	{
		count++;
		FStatusInstance* inst = *it;
		if (inst->timer > 0) // don't count down perma-buffs with timer = -1
		{
			inst->timer -= DeltaSeconds;
			if (inst->timer <= 0)
				inst->status = nullptr; // invalid and move it out of active elements
		}
	}

	// add status effects regularly, more often when the pool is low
	if (rand()%100 > count)
	{
		int index = rand()% StatusLibrary.Num();
		UStatus* status = StatusLibrary[index];
		FStatusInstance inst;
		inst.status = status;
		inst.timer = status->Duration;
		Statuses.AddItem(inst);
	}
	
	// drive the status effect pool and its display
	if (StatusPoolUI)
	{
		StatusPoolUI->SetPool(&Statuses);
		StatusPoolUI->Update();
	}
}

void APoolsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// press 1 and 2 to switch items and change the buff they give you 
	PlayerInputComponent->BindAction("Item1", IE_Released, this, &APoolsCharacter::EquipItem1);
	PlayerInputComponent->BindAction("Item2", IE_Released, this, &APoolsCharacter::EquipItem2);
}

// this simulates having a Status imbued by an item, so it never expires and only removes itself
// when the player swaps out the item.
void APoolsCharacter::EquipItem(FStatusInstance*& inst)
{
	// set the current buff to expire (if there is one)
	if (inst != nullptr)
		inst->timer = 0.01f;

	// pick a random buff, set it to -1 timer to never expire, and color it, so we can see it
	int index = rand()% StatusLibrary.Num();
	UStatus* status = StatusLibrary[index];
	FStatusInstance newInst;
	newInst.status = status;
	newInst.timer = -1; // never expires
	newInst.Color = inst == Item1Buff ? FColor::Red : FColor::Yellow; // mark it with a color

	// add to list and store the pointer to the element with our pool
	inst = Statuses.AddItem(newInst);
}
