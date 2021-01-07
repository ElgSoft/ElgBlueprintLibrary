// Copyright 2019-2020 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Engine/DataTable.h>
#include "ElgBP_DataTable.generated.h"

/**
 * 
 */
UCLASS()
class UElgBP_DataTable : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Add or Edit a row in the data table */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "DataTable", meta = (CustomStructureParam = "InAnyStruct"))
		static bool AddRowToDataTable(UDataTable* InDataTable, FName InRowName, UStruct* InAnyStruct);

	static bool Generic_AddRowToDataTable(UDataTable* InDataTable, FName InRowName, void* StructPtr);

	/** Based on UElgBP_DataTable::AddRowToDataTable */
	DECLARE_FUNCTION(execAddRowToDataTable) {
		P_GET_OBJECT(UDataTable, InDataTable);
		P_GET_PROPERTY(FNameProperty, InRowName);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* StructPtr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		UScriptStruct* InAnyStruct = StructProp->Struct;

		P_FINISH;

		bool bSuccess = false;
		if (!InDataTable) {
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation, FText::FromString("Failed to resolve the table input. Be sure the InDataTable is valid.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}  else if (!InAnyStruct) {
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation, FText::FromString("Failed to resolve the struct input. Be sure the InAnyStruct is valid.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		} else if (InRowName.IsNone()) {
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation, FText::FromString("Be sure the InRowName is valid.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		} else {

			const UScriptStruct* TableType = InDataTable->GetRowStruct();

			const bool bCompatible = (InAnyStruct == TableType) || 	(InAnyStruct->IsChildOf(TableType) && FStructUtils::TheSameLayout(InAnyStruct, TableType));
			if (bCompatible) {
				P_NATIVE_BEGIN;
				bSuccess = Generic_AddRowToDataTable(InDataTable, InRowName, StructPtr);
				P_NATIVE_END;
			} else {
				FBlueprintExceptionInfo ExceptionInfo(
					EBlueprintExceptionType::AccessViolation, FText::FromString("The InAnyStruct is incompatible with the data table's struct.")
				);
				FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
			}
		}	
		
		*(bool*)RESULT_PARAM = bSuccess;
	}

#pragma endregion

};
