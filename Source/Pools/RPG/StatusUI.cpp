// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusUI.h"

#include <string>

#include "Status.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UStatusUI::SetStatus(FStatusInstance* inst)
{
	Icon->SetBrushFromTexture(inst->status == nullptr ? Empty : inst->status->Icon);
	Icon->SetColorAndOpacity(inst->Color);
	Timer->SetText(FText::FromString(FString::FromInt(inst->timer)));
	// hide timer on expired or negatives
	Timer->SetVisibility(inst->timer > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
