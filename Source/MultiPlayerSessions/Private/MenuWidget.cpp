
#include "MenuWidget.h"
#include "MultiPlayerSessions/Public/MultiPlayerSessionSubSystem.h"
#include "Components/Button.h"


void UMenuWidget::MenuSetup(int32 NumPublicConnections,FString MatchTyp)
{
	this->NumberPublicConnections = NumPublicConnections;
	this->MatchType = MatchTyp;
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
			World->ServerTravel("/Game/ThirdPerson/Maps/LobbyLevel?listen");
		}
	}
}


void UMenuWidget::HostButtonClicked()
{
		// create session 
		if (PlayerSessionSubSystem)
		{
			PlayerSessionSubSystem->CreateSession(NumberPublicConnections,MatchType);
			
		}
}

void UMenuWidget::JoinButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Red,FString("Join Button"));

}
