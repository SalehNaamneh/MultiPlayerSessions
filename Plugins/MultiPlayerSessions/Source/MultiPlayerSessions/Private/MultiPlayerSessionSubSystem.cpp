
#include "MultiPlayerSessionSubSystem.h"

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

void UMultiPlayerSessionSubSystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
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
