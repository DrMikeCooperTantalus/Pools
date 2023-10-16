// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusUI.generated.h"

/**
 * 
 */
UCLASS()
class POOLS_API UStatusUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* Timer;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UImage* Icon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* Empty;
	
	void SetStatus(struct FStatusInstance* inst);
	
protected:
	struct FStatusInstance* status;
};
