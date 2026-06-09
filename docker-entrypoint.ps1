# docker-entrypoint.ps1 — launches the dedicated Ultima VI Online host and
# streams its log to the container's stdout so failures are visible in
# `docker logs` instead of the container exiting silently.
#
# Why this wrapper exists:
#   * The host is a /SUBSYSTEM:WINDOWS (GUI-subsystem) binary that allocates its
#     own console (CONSOLE build define). That console is NOT wired to the
#     container's stdout, so without this wrapper a crash during startup (e.g. a
#     missing data file under .\dr, .\host, .\ultima6 or .\save) just exits the
#     container with no output.
#   * Tailing log.txt (which the host writes via its scrlog file logger) gives
#     live startup progress and surfaces where setup failed.
#   * Tying the container's lifetime to the process and propagating its exit
#     code lets orchestrators (k8s, compose) see the real status.

$ErrorActionPreference = 'Stop'

$Root = 'C:\host'
$Exe  = Join-Path $Root 'Ultima VI Online Host.exe'
$Log  = Join-Path $Root 'log.txt'

if (-not (Test-Path -LiteralPath $Exe)) {
    Write-Error "Host executable not found at '$Exe'. Build the 'host' target in Release and rebuild the image."
    exit 1
}

# Pre-create the log so the reader can attach immediately; the host opens it
# OF_CREATE-style on startup and keeps appending.
New-Item -ItemType File -Path $Log -Force | Out-Null

Write-Output "Starting Ultima VI Online host from '$Root'..."
$proc = Start-Process -FilePath $Exe -WorkingDirectory $Root -PassThru

# Stream the log with ReadWrite sharing so we can read while the host writes.
$stream = [System.IO.File]::Open($Log, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::ReadWrite)
$reader = New-Object System.IO.StreamReader($stream)
try {
    while (-not $proc.HasExited) {
        $line = $reader.ReadLine()
        if ($null -ne $line) { Write-Output $line }
        else { Start-Sleep -Milliseconds 250 }
    }
    # Ensure the process object is finalized so ExitCode is populated.
    $proc.WaitForExit()
    # Drain anything written between the last read and process exit.
    while ($null -ne ($line = $reader.ReadLine())) { Write-Output $line }
}
finally {
    $reader.Dispose()
    $stream.Dispose()
}

Write-Output "Ultima VI Online host exited with code $($proc.ExitCode)."
exit $proc.ExitCode

