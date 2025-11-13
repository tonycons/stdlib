try {
    & "ninja" @("-C", "build")
} catch {
    Write-Error "Script failed: $($error[0].Message)"
    # Handle failure logic here
} finally {
    if ($?) {
        Start-Process -FilePath "build/voxels.exe" -NoNewWindow -Wait
        # Read-Host -Prompt "Press Enter to exit"
    }
}
