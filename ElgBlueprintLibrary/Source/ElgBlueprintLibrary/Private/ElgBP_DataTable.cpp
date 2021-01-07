// Copyright 2019-2020 ElgSoft. All rights reserved. 


#include "ElgBP_DataTable.h"
#include <Engine/DataTable.h>



bool UElgBP_DataTable::AddRowToDataTable(UDataTable* InDataTable, FName InRowName, UStruct* InAnyStruct)
{
	// dummy function, it's Generic_AddRowToDataTable that are called for real
	return true;
}

bool UElgBP_DataTable::Generic_AddRowToDataTable(UDataTable* InDataTable, FName InRowName, void* StructPtr)
{
	FTableRowBase* TableRowBase = (FTableRowBase*)StructPtr;
	InDataTable->AddRow(InRowName, *TableRowBase);
#if WITH_EDITOR
	// let the editor know that the data table has changed so it can get it's star.
	InDataTable->Modify();  
#endif
	return true;
}

