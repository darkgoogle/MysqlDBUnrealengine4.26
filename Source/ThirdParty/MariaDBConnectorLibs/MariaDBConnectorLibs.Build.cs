// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class MariaDBConnectorLibs: ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "/MariaDB_Connector_C_64-bit")); }
	}

	public MariaDBConnectorLibs(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "MariaDB_Connector_C_64-bit", "include"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			//PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "MariaDB_Connector_C_64-bit/lib", "lib")); 
			//PublicAdditionalLibraries.Add("libmariadb.lib");
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "MariaDB_Connector_C_64-bit/lib", "mariadbclient.lib"));	
			
			// Delay-load the DLL, so we can load it from the right place first	
			//PublicDelayLoadDLLs.Add("libmariadb.dll");				
			
			//PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "MariaDB Connector C 64-bit", "lib", "plugin"));
			// Delay-load the DLL, so we can load it from the right place first		
			//PublicDelayLoadDLLs.Add("dialog.dll");
			//PublicDelayLoadDLLs.Add("mysqlclearpassword.dll");
					
		}	
	}
}
