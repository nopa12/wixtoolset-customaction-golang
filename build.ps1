# get wix toolset
$zipUrl = "https://github.com/wixtoolset/wix3/releases/download/wix3112rtm/wix311-binaries.zip"
$zipFilePath = "$PWD\wix311-binaries.zip"
$extractPath = "$PWD\wix311-binaries"

if (Test-Path -Path $extractPath) {
    Write-Host "wix311-binaries Directory already exists. Skipping download."
} else {
    Write-Host "Downloading wix toolset wix311-binaries..."
    Invoke-WebRequest -Uri $zipUrl -OutFile $zipFilePath
    
    Write-Host "Unpacking ZIP file..."
    Expand-Archive -Path $zipFilePath -DestinationPath $extractPath
}

# build go-dll, must be done before custom action. cause it will be a resource of custom action
cd go-dll
& ".\build.ps1"
cd ..\

# build custom action
cd c-custom-action
Start-Process -NoNewWindow -FilePath ".\build_test.cmd"
cd ..\

# Build object
$candleParams = "-nologo -v "`
                + "-out build\ "`
                + "wix-installer\Product.wxs "`

Start-Process -NoNewWindow -FilePath wix311-binaries\candle.exe -ArgumentList $candleParams -Wait

# Create MSI
$lightParams =  "-nologo -v "`
                + '-out "build\\installer.msi" '`
                + 'build\Product.wixobj '`

Start-Process -NoNewWindow -FilePath wix311-binaries\light.exe -ArgumentList $lightParams -Wait