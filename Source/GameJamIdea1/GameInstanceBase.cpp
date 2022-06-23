// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstanceBase.h"

void UGameInstanceBase::Init() {
  Super::Init();

  FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UGameInstanceBase::BeginLoadingScreen);
  FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameInstanceBase::EndLoadingScreen);
}

void UGameInstanceBase::BeginLoadingScreen(const FString& InMapName) {
  if (!IsRunningDedicatedServer()) {
    FLoadingScreenAttributes LoadingScreen;
    LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
    LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

    GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
  }
}

void UGameInstanceBase::EndLoadingScreen(UWorld* InLoadedWorld) {}