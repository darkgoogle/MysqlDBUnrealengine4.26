#pragma once

#include  "mysql.h"
#include  "MySQLConnection.h"
#include  "MySQLConnectorStructs.h"
#include  "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "Engine/NetConnection.h"
#include "MySQLDatabase.generated.h"


USTRUCT(BlueprintType)
struct MYSQLCONNECTORUE4PLUGIN_API FMySQLConnectorConnectionStruct
{
	GENERATED_USTRUCT_BODY()

		/** The database name (not the filename) */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySQLConnector Database Reference")
		UMySQLConnection* conn;

};

USTRUCT(BlueprintType)
struct MYSQLCONNECTORUE4PLUGIN_API FMySQLConnectorKeyValuePair
{
	GENERATED_USTRUCT_BODY()

		/** The database table field name */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySQLConnector Key Value Pair")
		FString Key;

	/** The value of the field */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySQLConnector Key Value Pair")
		FString Value;
};

USTRUCT(BlueprintType)
struct MYSQLCONNECTORUE4PLUGIN_API FMySQLConnectorQueryResultRow
{
	GENERATED_USTRUCT_BODY()

		/** A list of field name, field value pairs */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySQLConnector Query Result")
		TArray<FMySQLConnectorKeyValuePair> Fields;
};

USTRUCT(BlueprintType)
struct MYSQLCONNECTORUE4PLUGIN_API FMySQLConnectoreQueryResult
{
	GENERATED_USTRUCT_BODY()

		/** The resulting rows from the query */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySQLConnector Query Result")
		TArray<FMySQLConnectorQueryResultRow> ResultRows;

	/** Was the query successful or not */
	UPROPERTY(BlueprintReadOnly, Category = "MySQLConnector Query Result")
		bool Success;

	/** If the query was unsuccessful a human readable error message will be populated here */
	UPROPERTY(BlueprintReadOnly, Category = "MySQLConnector Query Result")
		FString ErrorMessage;

};


// A few things for internal use here.
namespace MySQLConnectorResultValueTypes
{
	enum MySQLConnectorResultValType
	{
		Int,
		Varchar,
		UnsupportedValueType
	};
}


// Result field, used as an intermediary when collecting results from
// an MySQL query.
struct MySQLConnectorResultField
{
	FString StringValue;
	double DoubleValue;
	int64 IntValue;

	FString Name;
	MySQLConnectorResultValueTypes::MySQLConnectorResultValType Type;

	FString ToString()
	{
		if (Type == MySQLConnectorResultValueTypes::Varchar)
			return StringValue;
		if (Type == MySQLConnectorResultValueTypes::Int)
			return FString::Printf(TEXT("%i"), IntValue);
		return StringValue;
	}
};

struct MySQLConnectorResultValue
{
	TArray<MySQLConnectorResultField> Fields;
};

// The internal result object.
struct MySQLConnectorQueryResult
{
	bool Success;
	FString ErrorMessage;
	TArray<MySQLConnectorResultValue> Results;
};






/**
* MySQLConnector main database class.
*/
UCLASS()
class MYSQLCONNECTORUE4PLUGIN_API UMySQLDatabase : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MySQLConnector")
		static UMySQLConnection* MySQLInitConnection(FString Host, FString UserName, FString UserPassword, FString DatabaseName);


	UFUNCTION(BlueprintCallable, Category = "MySQLConnector|Query")
		static bool MySQLConnectorExecuteQuery(FString Query, UMySQLConnection* Connection);

	/** Drop Table*/
	UFUNCTION(BlueprintCallable, Category = "MySQLConnector|Query", meta = (DisplayName = "Drop Table"))
		static bool DropTable(const FString TableName, UMySQLConnection* Connection);

	/** Truncate Table*/
	UFUNCTION(BlueprintCallable, Category = "MySQLConnector|Query", meta = (DisplayName = "Truncate Table"))
		static bool TruncateTable(const FString TableName, UMySQLConnection* Connection);

	/** Create table in the database. */
	UFUNCTION(BlueprintCallable, Category = "MySQLConnector|Query", meta = (DisplayName = "Create Table"))
		static FMySQLConnectorTable CreateTable(const FString TableName,
			const TArray<FMySQLConnectorTableField> Fields, UMySQLConnection* Connection);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "INT (MySQLConnector)"), Category = "MySQLConnector|Query|DataTypes")
		static FMySQLConnectorTableField MySQLConnectorINT(FString FieldName, const bool PK, const bool AI);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "VARCHAR (MySQLConnector)"), Category = "MySQLConnector|Query|DataTypes")
		static FMySQLConnectorTableField MySQLConnectorVARCHAR(const FString FieldName, const FString FieldLength,
			const bool PK, const bool Unique, const bool NotNull);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "InsertTest"), Category = "MySQLConnector|Query")
		static bool MySQLConnectorInsertTest(const FString Query, UMySQLConnection* Connection);

	/** returns the address or empty String on error, returns address as Integer or -1 on error */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Controller IP-Address"), Category = "MySQLConnector|Connection")
	static bool MySQLConnectorGetPlayerControllerIP(FString& IP, int32& IP_int, APlayerController* PlayerController);

	/** Get data from the database using a select statement and return the rows. */
	UFUNCTION(BlueprintCallable, Category = "MySQLConnector|Query", meta = (DisplayName = "Get Data From Table(s) (manual query)"))
		static FMySQLConnectoreQueryResult MySQLConnectorGetData(const FString& Query, UMySQLConnection* Connection);

private:
	/** Runs a query and returns fetched rows. */
	static MySQLConnectorQueryResult RunQueryAndGetResults(FString Query, UMySQLConnection* Connection);




};
