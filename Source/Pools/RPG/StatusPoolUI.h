// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Pools/TPool.h"
#include "StatusPoolUI.generated.h"

constexpr int RowSize = 16;

/**
 * 
 */
UCLASS()
class POOLS_API UStatusPoolUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UPanelWidget* Container;
	// UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	// class UPanelWidget* Row;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UStatusUI* Prefab;

	void SetPool(TPool<struct FStatusInstance, 16>* pool);
	
	void Update();
	
protected:
	TPool<struct FStatusInstance, RowSize>* Pool;

	//TArray<class UPanelWidget*> Rows;
	TArray<class UStatusUI*> UIs;
};
