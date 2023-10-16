// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Status.generated.h"

/**
 * 
 */
UCLASS()
class POOLS_API UStatus : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UTexture2D* Icon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Duration;
};

struct FStatusInstance
{
	FStatusInstance() { status = nullptr; timer = 0; Color = FColor::White; }
	class UStatus* status;
	float timer = 0;
	FColor Color;
	
	bool IsValid() { return status != nullptr; }
};
