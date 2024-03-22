// Fill out your copyright notice in the Description page of Project Settings.


#include "SubmarineGameInstance.h"

bool USubmarineGameInstance::GetInfluenceInfoByClass(TSubclassOf<AActor> Actor, FInfluenceInfo& OutInfo)
{
    bool bIsFound = false;
    
    if (InfluenceDataTable)
    {
        TArray<FName> NameArray = InfluenceDataTable->GetRowNames();
        int8 i = 0;
        while (i < NameArray.Num() && !bIsFound)
        {
            FInfluenceInfo* Row = InfluenceDataTable->FindRow<FInfluenceInfo>(NameArray[i], "");
            if (Row->Class == Actor)
            {
                bIsFound = true;
                OutInfo = *Row;
            }
            else
                i++;
        }
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("USubmarineGameInstance::GetInfluenceInfoByClass - InfluenceTable -NULL"));

    return bIsFound;
}

bool USubmarineGameInstance::GetLivingCreatureInfoByClass(TSubclassOf<AActor> Actor, FLivingCreatureInfo& OutInfo)
{
    bool bIsFound = false;

    if (LivingCreatureTable)
    {
        TArray<FName> NameArray = LivingCreatureTable->GetRowNames();
        int8 i = 0;
        while (i < NameArray.Num() && !bIsFound)
        {
            FLivingCreatureInfo* Row = LivingCreatureTable->FindRow<FLivingCreatureInfo>(NameArray[i], "");
            if (Row->Class == Actor)
            {
                bIsFound = true;
                OutInfo = *Row;
            }
            else
                i++;
        }
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("USubmarineGameInstance::GetInfluenceInfoByClass - LivingCreatureTable -NULL"));

    return bIsFound;
}
