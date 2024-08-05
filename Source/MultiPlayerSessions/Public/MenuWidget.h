// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumPublicConnections = 8 ,FString MatchType = "FreeForAll" ,FString lobbyPath = "/Game/ThirdPerson/Maps/LobbyLevel?listen");
    void MenuTearDown();
protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;


	/// call backs for custom delegates in menu sub system
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionSearchResult , bool bWasSuccsseful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	
	UFUNCTION()
	void OnDestroySession(bool bWasSuccsseful);
	
	UFUNCTION()
	void OnStartSession(bool bWasSuccsseful);
private:
	// Host Button to bind to the host button in unreal engine
	UPROPERTY(meta=(BindWidget))
	class UButton * HostButton;
	
	// Join Button to bind to the host button in unreal engine
	UPROPERTY(meta=(BindWidget))
	class UButton * JoinButton;


	// CallBack Func to bind for the Buttons
	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	// Our SubSystem to handle the subsystem throw the menu Widget
	class UMultiPlayerSessionSubSystem * PlayerSessionSubSystem;
	int32 NumberPublicConnections{8};
	FString MatchType{TEXT("FreeForAll")};
	FString PathToLobby{""};
};
