// Fill out your copyright notice in the Description page of Project Settings.


#include "MigrationPoint.h"

// Sets default values
AMigrationPoint::AMigrationPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

int32 AMigrationPoint::GetMyLevel()
{
	return myLevel;
}

// Called when the game starts or when spawned
void AMigrationPoint::BeginPlay()
{
	Super::BeginPlay();
	
}


