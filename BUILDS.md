# Build with Visual Studio

## Prerequisites

- **Visual Studio 2022** (Community, Professional, or Enterprise) with the **Desktop development with C++** workload installed.
- **Dokan v2.3.1** driver and library — required by the `fsxfsmount` project to expose mounted XFS filesystems as Windows drive letters.
- **Python 3.11+** — used by the `build.ps1` script to convert the `msvscpp` into Visual Studio solution format.

## Step 1: Install the Dokan Driver and Library

1. Download the **DokanSetup.exe** installer from the [Dokany releases page](https://github.com/dokan-dev/dokany/releases) (version **2.3.1**).
2. Run the installer and select **both** components:
   - **Dokan User-Mode Library** — installs headers and `.lib` files.
   - **Dokan Kernel Driver** — installs the filesystem driver (`dokan2.sys`).
3. Use the default installation path: `C:\Program Files\Dokan\DokanLibrary-2.3.1`.

After installation, verify the following paths exist:

| File | Path |
|------|------|
| Header | `C:\Program Files\Dokan\DokanLibrary-2.3.1\include\dokan\dokan.h` |
| x64 lib | `C:\Program Files\Dokan\DokanLibrary-2.3.1\lib\dokan2.lib` |
| x86 lib | `C:\Program Files\Dokan\DokanLibrary-2.3.1\x86\lib\dokan2.lib` |

> **Note:** If you install a different Dokan version or to a non-default path, you must update the `AdditionalIncludeDirectories` and `AdditionalDependencies` in `vs2022\fsxfsmount\fsxfsmount.vcxproj` to match.

## Step 2: Sync dependencies libraries and autogen necessary files
```powershell
.\synclib.ps1
.\autogen.ps1
```

## Step 3: Generate the VS2022 Solution and Build

This step converts the base Visual Studio solution to VS2022 format and compiles all projects.

```powershell
.\build.ps1 -PythonPath <your_python_path> -Configuration "Release" -Platform "x64"
```

The default expected python path is `C:\Python311`, if your Python is installed in a different location (e.g., `C:\Python312` or `C:\Users\<username>\AppData\Local\Programs\Python\Python311`), specify the correct path.

**Note:** Ignore the build error when compiling `pyfsxfs` project because we don't need it for CLI tools

## Step 4: Fix fsxfsmount project to link to installed dokan library

Open the solution in Visual Studio and update the `fsxfsmount` project settings to match your Dokan installation.

1. Open `vs2022\libfsxfs.sln` in Visual Studio 2022.
2. In **Solution Explorer**, right-click **fsxfsmount** and choose **Properties**.
3. Set **Configuration** = `All Configurations` and **Platform** = `All Platforms`.
4. Update the following fields (paths shown for the default Dokan install):

    - **C/C++ > General > Additional Include Directories**
       `C:\Program Files\Dokan\DokanLibrary-2.3.1\include\dokan`

    - **Linker > General > Additional Library Directories**
       - x64: `C:\Program Files\Dokan\DokanLibrary-2.3.1\lib`
       - Win32: `C:\Program Files\Dokan\DokanLibrary-2.3.1\x86\lib`

    - **Linker > Input > Additional Dependencies**
       `dokan2.lib`

5. Save the project and rebuild `fsxfsmount`.

> If Dokan is installed in a different location or version, replace the paths above with your install path.


## Step 5: Verify the Build Output

Build artifacts are placed in `vs2022\Release\<Platform>\`.

```
vs2022\Release\x64\fsxfsmount.exe    # XFS mount tool (uses Dokan)
vs2022\Release\x64\fsxfsinfo.exe     # XFS info tool
vs2022\Release\x64\libfsxfs.dll      # Core library
```

Test that `fsxfsmount.exe` loads correctly:

```powershell
.\vs2022\Release\x64\fsxfsmount.exe -h
```

## How Dokan is Linked to `fsxfsmount`

The `fsxfsmount` project (`vs2022\fsxfsmount\fsxfsmount.vcxproj`) is pre-configured to link against Dokan:

- **Include path:** `C:\Program Files\Dokan\DokanLibrary-2.3.1\include\dokan` is added to `AdditionalIncludeDirectories`.
- **Preprocessor:** `HAVE_LIBDOKAN` is defined, which enables the Dokan code path in `mount_dokan.c` (guarded by `#if defined( HAVE_LIBDOKAN )`).
- **Linker:** `dokan2.lib` is added to `AdditionalDependencies` (x86 uses `x86\lib\dokan2.lib`, x64 uses `lib\dokan2.lib`).
- **Runtime DLL:** At runtime, `fsxfsmount.exe` requires `dokan2.dll` to be on the system `PATH` (the Dokan installer handles this).

## Step 6: Build xfs_extract.exe

`xfs_extract.exe` is a standalone tool that orchestrates the full extraction workflow: it launches `fsxfsmount.exe` in the background, copies all content to an output directory, then terminates the mount process and cleans up.

The project is already included in `msvcpp\libfsxfs.sln` and depends on the `fsxfsmount` project, so **Steps 3 and 4 must be completed first**.

### Build inside Visual Studio

1. Open `vs2022\libfsxfs.sln` in Visual Studio 2022.
2. In **Solution Explorer**, right-click **fsxfsextract** and choose **Build**.
3. The output is placed alongside the other tools:
   
```
vs2022\Release\x64\xfs_extract.exe
```

### Verify the build output

```powershell
.\vs2022\Release\x64\xfs_extract.exe -h
```

### Usage

```
xfs_extract [-v] <xfs_image> <output_dir> [path_to_fsxfsmount.exe]
   -v flag for verbose output to a log.txt
```

Example:

```powershell
.\vs2022\Release\x64\xfs_extract.exe `
    C:\images\disk.xfs `
    C:\output\extracted
```

`xfs_extract.exe` expects `fsxfsmount.exe` to be in the same directory by default. If it is located elsewhere, pass the full path as the third argument.

> **Note:** `xfs_extract.exe` does **not** require Dokan headers or libraries at compile time. It only requires `fsxfsmount.exe` (and by extension the Dokan driver) to be present at **runtime**.

## Troubleshooting

| Problem | Solution |
|---------|----------|
| `dokan.h` not found | Check that the Dokan include path in the vcxproj matches your installed version. |
| `dokan2.lib` linker error | Verify the `.lib` path. x64 builds need `lib\dokan2.lib`, x86 builds need `x86\lib\dokan2.lib`. |
| `dokan2.dll` not found at runtime | Ensure the Dokan driver is installed and `C:\Program Files\Dokan\DokanLibrary-2.3.1` is on your system `PATH`. |

| `fsxfsmount` hangs on exit | The Dokan kernel driver may not be running. Start it with `sc start dokan2` (requires admin). |
