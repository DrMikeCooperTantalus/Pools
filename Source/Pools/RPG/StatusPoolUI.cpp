// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusPoolUI.h"
#include "Components/PanelWidget.h"
#include "Status.h"
#include "StatusUI.h"

void UStatusPoolUI::SetPool(TPool<FStatusInstance, 16>* pool)
{
	Pool = pool;
}

void UStatusPoolUI::Update()
{
	const TArray<FStatusInstance*>& Pages = Pool->GetPages();

	// add new rows as needed
	while (UIs.Num() < Pages.Num() * RowSize)
	{
		for (int j=0; j< RowSize; j++)
		{
			UStatusUI* ui = DuplicateObject<UStatusUI>(Prefab, Container);
			Container->AddChild(ui);
			UIs.Add(ui);
		}
	}
	for (int i=0; i< Pages.Num(); i++)
	{
		// this will update the image and timer
		for (int j=0; j< RowSize; j++)
			UIs[i*RowSize+j]->SetStatus(&Pages[i][j]);
	}
}
