
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
		OnlineSessionPtr->DestroySession(NAME_GameSession);
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
}

void UMultiPlayerSessionSubSystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
}

void UMultiPlayerSessionSubSystem::DestroySession()
{
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
}

void UMultiPlayerSessionSubSystem::OnJoinSessionComplete(FName MatchName, EOnJoinSessionCompleteResult::Type JoinType)
{
}

void UMultiPlayerSessionSubSystem::OnDestroySessionComplete(FName MatchName, bool bWasSuccessful)
{
}

void UMultiPlayerSessionSubSystem::OnStartSessionComplete(FName MatchName, bool bWasSuccessful)
{
}
