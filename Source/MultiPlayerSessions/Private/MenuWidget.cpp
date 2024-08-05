
#include "MenuWidget.h"

#include "OnlineSessionSettings.h"
#include "MultiPlayerSessions/Public/MultiPlayerSessionSubSystem.h"
#include "Components/Button.h"


void UMenuWidget::MenuSetup(int32 NumPublicConnections,FString MatchTyp,FString lobbyPath)
{
	this->NumberPublicConnections = NumPublicConnections;
	this->MatchType = MatchTyp;
	PathToLobby = FString::Printf(TEXT("%s?listen"),*lobbyPath);
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld *World = GetWorld();
	if (World)
	{
		APlayerController * PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
		}
	}
	// Access the our subsystem
	UGameInstance * GameInstance = GetGameInstance();
	if (GameInstance)
	{
		PlayerSessionSubSystem = GameInstance->GetSubsystem<UMultiPlayerSessionSubSystem>();
	}

	if (!IsValid(PlayerSessionSubSystem)) return;
	PlayerSessionSubSystem->MultiPlayerOnCreateSessionComplete.AddDynamic(this,&ThisClass::UMenuWidget::OnCreateSession);
	PlayerSessionSubSystem->MultiPlayerOnFindSessionComplete.AddUObject(this,&ThisClass::OnFindSession);
	PlayerSessionSubSystem->MultiPlayerOnJoinSessionComplete.AddUObject(this,&ThisClass::OnJoinSession);
	PlayerSessionSubSystem->MultiPlayerOnDestroySessionComplete.AddDynamic(this,&ThisClass::UMenuWidget::OnDestroySession);
	PlayerSessionSubSystem->MultiPlayerOnStartSessionComplete.AddDynamic(this,&ThisClass::UMenuWidget::OnStartSession);
}

void UMenuWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

bool UMenuWidget::Initialize()
{
	// here bind the button with the call back function 
	if (!Super::Initialize())
	{
		return false;
	}
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this,&ThisClass::HostButtonClicked);	
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this,&ThisClass::JoinButtonClicked);	
	}
	return true;
	
}

void UMenuWidget::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

// call back function
void UMenuWidget::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Green,FString("Create Session Successful"));
		}
		// travel to the lobby
		UWorld * World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		HostButton->SetIsEnabled(true);
	}
}

void UMenuWidget::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionSearchResult, bool bWasSuccsseful)
{
	
		for (auto SessionResult : SessionSearchResult)
		{
			FString SettingsValue;
			SessionResult.Session.SessionSettings.Get(FName("MatchType"),SettingsValue);
			if (SettingsValue == MatchType)
			{
				if (PlayerSessionSubSystem)
				{
					PlayerSessionSubSystem->JoinSession(SessionResult);
					return;
				}
			}
		}
	
	if (!bWasSuccsseful || SessionSearchResult.Num() == 0 )
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem * Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			FString Address;
			Session->GetResolvedConnectString(NAME_GameSession,Address);
			APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address , TRAVEL_Absolute);
			}
		}
	}
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMenuWidget::OnDestroySession(bool bWasSuccsseful)
{
}

void UMenuWidget::OnStartSession(bool bWasSuccsseful)
{
}


void UMenuWidget::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
		// create session 
		if (PlayerSessionSubSystem)
		{
			PlayerSessionSubSystem->CreateSession(NumberPublicConnections,MatchType);
			
		}
}

void UMenuWidget::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	if (PlayerSessionSubSystem)
	{
		PlayerSessionSubSystem->FindSession(10000);
	}
}
