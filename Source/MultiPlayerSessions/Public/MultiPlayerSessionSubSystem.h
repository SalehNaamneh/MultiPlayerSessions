// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiPlayerSessionSubSystem.generated.h"

/**
 * 
 */

/// custom Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiPlayerOnCreateSessionComplete,bool ,bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiPlayerOnFindSessionComplete , const TArray<FOnlineSessionSearchResult>& SessionSearchResult , bool bWasSuccsseful );
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiPlayerOnJoinSessionComplete , EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiPlayerOnDestroySessionComplete , bool , bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiPlayerOnStartSessionComplete , bool , bWasSuccessful);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiPlayerSessionSubSystem : public UGameInstanceSubsystem

{
	GENERATED_BODY()
public:
	UMultiPlayerSessionSubSystem();
	void CreateSession(int32 NumPublicConnections , FString MatchType);
	void FindSession(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	void DestroySession();
	void StartSession();



	// Custom delegate to bind to the call backs functions

	FMultiPlayerOnCreateSessionComplete MultiPlayerOnCreateSessionComplete;
	FMultiPlayerOnFindSessionComplete MultiPlayerOnFindSessionComplete;
	FMultiPlayerOnJoinSessionComplete MultiPlayerOnJoinSessionComplete;
	FMultiPlayerOnDestroySessionComplete MultiPlayerOnDestroySessionComplete;
	FMultiPlayerOnStartSessionComplete MultiPlayerOnStartSessionComplete;
protected:
	// create callbacks func for every delegate
	
	// create a callback func to bind to the delegate 
	void OnCreateSessionComplete(FName SessionName,bool bWasSuccessful);
	// create a callback func to bind to the delegate  for find session
	void OnFindSessionComplete(bool bWasSuccessful);
	// create a callback func to bind to the delegate for join session
	void OnJoinSessionComplete(FName MatchName , EOnJoinSessionCompleteResult::Type JoinType);
	// create a callback func to bind to destroy delegate
	void OnDestroySessionComplete(FName MatchName , bool bWasSuccessful );
	// create a callback func to bind to start delegate
	void OnStartSessionComplete(FName MatchName , bool bWasSuccessful );
private:
	IOnlineSessionPtr OnlineSessionPtr;

	// add delegate for every func create start etc ...
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;

	// Create FDelegate Handle for every delegate
	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle FindSessionDelegateHandle;
	FDelegateHandle JoinSessionDelegateHandle;
	FDelegateHandle DestroySessionDelegateHandle;
	FDelegateHandle StartSessionDelegateHandle;


	TSharedPtr<FOnlineSessionSettings>  SessionSettings;
	TSharedPtr<FOnlineSessionSearch> SearchSettings;

	bool bCreateSessionOnDestroy{false};
	int32 LastNumPublicConnection;
	FString LastMatchType;
	
};
