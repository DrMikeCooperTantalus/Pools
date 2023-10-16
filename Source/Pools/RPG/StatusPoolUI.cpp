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
	for (int i=0; i< Pages.Num(); i++)
	{
		// clone more rows if necessary
		if (i >= Rows.Num())
		{
			UPanelWidget* row = Row;
			if (i != 0)
			{
				row = DuplicateObject<UPanelWidget>(Row, Container);
				Container->AddChild(row);
			}
			Rows.Add(row);

			if (i==0)
			{
				// now clone 16 status UIs for this row
				for (int j=0; j< RowSize; j++)
				{
					UStatusUI* ui = DuplicateObject<UStatusUI>(Prefab, row);
					row->AddChild(ui);
					UIs.Add(ui);
				}
			}
			else
			{
				// add existing uis into our array (they'll have come with th clone)
				for (int j=0; j<RowSize; j++)
				{
					UStatusUI* ui = dynamic_cast<UStatusUI*>(row->GetChildAt(j));
					if (ui)
						UIs.Add(ui);
				}
			}
		}

		// this will update the image and timer
		for (int j=0; j< RowSize; j++)
			UIs[i*RowSize+j]->SetStatus(&Pages[i][j]);
	}
}
