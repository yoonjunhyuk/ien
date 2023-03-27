#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/Data/MyCharacterData.MyCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MyChar(*CharacterDataPath);
	MYCHECK(DT_MyChar.Succeeded());
	MyCharacterTable = DT_MyChar.Object;
	MYCHECK(MyCharacterTable->RowMap.Num() > 0);
}

void UMyGameInstance::Init()
{
	Super::Init();
}

FMyCharacterData* UMyGameInstance::GetMyCharacterData(int32 Level)
{
	return MyCharacterTable->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}