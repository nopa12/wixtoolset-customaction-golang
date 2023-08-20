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