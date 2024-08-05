
#include "MultiPlayerSessionSubSystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

UMultiPlayerSessionSubSystem::UMultiPlayerSessionSubSystem():
CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnCreateSessionComplete)),
FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,&ThisClass::OnFindSessionComplete)),
JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnJoinSessionComplete)),
DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this,&ThisClass::OnDestroySessionComplete)),
StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem *SubSystem = IOnlineSubsystem::Get();
	if (SubSystem)
	{
		OnlineSessionPtr = SubSystem->GetSessionInterface();
	}
}

void UMultiPlayerSessionSubSystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!OnlineSessionPtr.IsValid()) return;

	auto ExistingSession = OnlineSessionPtr->GetNamedSession(NAME_GameSession);
	if (ExistingSession)
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnection = NumPublicConnections;
		LastMatchType = MatchType;
		this->DestroySession();
	}
	// store the delegate to delegate handle
	CreateSessionDelegateHandle = OnlineSessionPtr->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	
	SessionSettings =  MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NUll" ? true : false;
	SessionSettings->NumPublicConnections = NumPublicConnections;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(FName("MatchType"),MatchType,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->BuildUniqueId = 1 ;

	const ULocalPlayer * LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!OnlineSessionPtr->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(),NAME_GameSession,*SessionSettings))
	{
		OnlineSessionPtr->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);

		// BroadCast are own custom Delegate
		MultiPlayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UMultiPlayerSessionSubSystem::FindSession(int32 MaxSearchResults)
{
	if (!OnlineSessionPtr.IsValid()) return;
	FindSessionDelegateHandle = OnlineSessionPtr->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	SearchSettings = MakeShareable(new FOnlineSessionSearch);
	SearchSettings->MaxSearchResults = MaxSearchResults;
	SearchSettings->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false ;
	const ULocalPlayer * LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (! OnlineSessionPtr->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(),SearchSettings.ToSharedRef()))
	{
		OnlineSessionPtr->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionDelegateHandle);
		MultiPlayerOnFindSessionComplete.Broadcast(TArray<FOnlineSessionSearchResult>() , false);
	}
}

void UMultiPlayerSessionSubSystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (!OnlineSessionPtr.IsValid())
	{
		MultiPlayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	OnlineSessionPtr->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
	const ULocalPlayer * LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(!OnlineSessionPtr->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(),NAME_GameSession,SearchResult))
	{
		OnlineSessionPtr->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
		MultiPlayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMultiPlayerSessionSubSystem::DestroySession()
{
	if (!OnlineSessionPtr.IsValid())
	{
		MultiPlayerOnDestroySessionComplete.Broadcast(false);
		return;
	}
	OnlineSessionPtr->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
	if(!OnlineSessionPtr->DestroySession(NAME_GameSession))
	{
		OnlineSessionPtr->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
		MultiPlayerOnDestroySessionComplete.Broadcast(false);
	}
	
}

void UMultiPlayerSessionSubSystem::StartSession()
{
}



// call backs function to the delegates 
void UMultiPlayerSessionSubSystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSessionPtr.IsValid())
	{
		OnlineSessionPtr->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
	}
	MultiPlayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiPlayerSessionSubSystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (OnlineSessionPtr)
	{
		OnlineSessionPtr->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionDelegateHandle);
		if (SearchSettings->SearchResults.Num() == 0)
		{
			MultiPlayerOnFindSessionComplete.Broadcast(SearchSettings->SearchResults,false);
		}
		MultiPlayerOnFindSessionComplete.Broadcast(SearchSettings->SearchResults,bWasSuccessful);
	}
}

void UMultiPlayerSessionSubSystem::OnJoinSessionComplete(FName MatchName, EOnJoinSessionCompleteResult::Type JoinType)
{
	if (!OnlineSessionPtr)
	{
		OnlineSessionPtr->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	}
	MultiPlayerOnJoinSessionComplete.Broadcast(JoinType);
}

void UMultiPlayerSessionSubSystem::OnDestroySessionComplete(FName MatchName, bool bWasSuccessful)
{
	if (OnlineSessionPtr)
	{
		OnlineSessionPtr->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
	}
	if (bWasSuccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		CreateSession(LastNumPublicConnection,LastMatchType);
	}
	MultiPlayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiPlayerSessionSubSystem::OnStartSessionComplete(FName MatchName, bool bWasSuccessful)
{
}
