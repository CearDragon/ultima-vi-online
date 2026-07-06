# Probe game-host ports: connect, read first bytes (host sends 4-byte U6O_SIGNATURE
# 'U6O2' immediately on accept), print hex+ascii. Usage: .\probe_ports.ps1 [-TargetHost ip] [ports...]
param([int[]]$Ports = @(22, 8080), [string]$TargetHost = '127.0.0.1')

foreach ($port in $Ports) {
    try {
        $c = New-Object Net.Sockets.TcpClient
        $ar = $c.BeginConnect($TargetHost, $port, $null, $null)
        if (-not $ar.AsyncWaitHandle.WaitOne(3000)) { throw "connect timeout" }
        $c.EndConnect($ar)
        $s = $c.GetStream()
        $s.ReadTimeout = 5000
        $buf = New-Object byte[] 16
        $n = 0
        try { $n = $s.Read($buf, 0, 16) } catch { }
        if ($n -gt 0) {
            $hex = ($buf[0..($n-1)] | ForEach-Object { $_.ToString('X2') }) -join ' '
            $ascii = -join ($buf[0..($n-1)] | ForEach-Object { if ($_ -ge 32 -and $_ -le 126) { [char]$_ } else { '.' } })
            Write-Output "$TargetHost port ${port}: read $n bytes: $hex  '$ascii'"
        } else {
            Write-Output "$TargetHost port ${port}: connected but server sent nothing within 5s"
        }
        $c.Close()
    } catch {
        Write-Output "$TargetHost port ${port}: FAILED - $($_.Exception.Message)"
    }
}
