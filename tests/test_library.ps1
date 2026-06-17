# Tests library functions and types.
#
# Version: 20260615

$LibraryTests = "attribute_values attributes attributes_leaf_block_header attributes_table attributes_table_header block_directory block_directory_footer block_directory_header btree_block btree_header buffer_data_handle directory_entry directory_table directory_table_header error extent file_system_block file_system_block_header inode_btree inode_btree_record inode_information io_handle notify superblock"
$LibraryTestsWithInput = "support volume"
$OptionSets = "offset" -split " "

. .\test_functions.ps1

Function RunTest
{
	param( [string]$TestName )

	$TestBinary = "fsxfs_test_${TestName}"

	$TestDescription = "${TestBinary}"
	$TestExecutable = "${TestExecutablesDirectory}\${TestBinary}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		WriteTestResult ${TestDescription} ${ExitIgnore}

		Return ${ExitIgnore}
	}
	$Output = Invoke-Expression ${TestExecutable}
	$Result = $global:LastExitCode

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

Function RunTestWithInput
{
	param( [string]$TestName, [string[]]$TestInput )

	$OptionSet = $TestInput[0]
	$Options = $TestInput[1]
	$TestFile = $TestInput[2]

	$TestBinary = "fsxfs_test_${TestName}"
	$TestFileName = (${TestFile} -split '\\')[-2..-1] -join '\'

	$TestDescription = "${TestBinary} with input: '${TestFileName}"
	$TestExecutable = "${TestExecutablesDirectory}\${TestBinary}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		WriteTestResult ${TestDescription} ${ExitIgnore}

		Return ${ExitIgnore}
	}
	$Output = Invoke-Expression "${TestExecutable} ${Options} ${TestFile}"
	$Result = $global:LastExitCode

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTest ${TestName}

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

$TestInputs = GenerateTestInputs "libfsxfs" ${OptionSets}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	ForEach ($TestInput in ${TestInputs})
	{
		$Result = RunTestWithInput ${TestName} ${TestInput}

		If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
		{
			Break
		}
	}
	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Exit ${Result}
