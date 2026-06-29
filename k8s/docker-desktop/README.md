# Running Ultima VI Online Host in Kubernetes

This directory contains the necessary files to run the Ultima VI Online Host in a local Kubernetes cluster (Docker Desktop).

## Prerequisites

1.  **Docker Desktop** with **Linux Containers** enabled.
    - Ensure Docker is set to Linux containers mode (default).
2.  **Kubernetes** enabled in Docker Desktop settings.
3.  **Map Data**: The host game data (including the original Ultima 6 map
    files such as `chunks`, `map`, etc.) must be provided in `C:\host` on
    your local machine.
    - **Required Structure**:
      ```text
      C:\host\
      ├── dr\             (objfloat.flg, u6spr.lnk, ...)
      ├── host\           (crtenum.bin, schedule.bin, ...)
      ├── ultima6\        (chunks, map, ...)
      │   └── savegame\   (objblkAA, ...)
      ├── dns.txt
      └── motd.txt
      ```
    - This directory is mounted to `/u6o-host/` in the container.

## Deployment Steps

### 1. Build the Docker Image

Build the **Docker Desktop / Kubernetes** image. Unlike the root `Dockerfile`,
this variant (`Dockerfile.docker-desktop`) bakes in **only the compiled host
binary** — no game data — so all host files are managed on disk via the `C:\host`
mount and you never rebuild the image just to edit them.

Run the following from the project root:

```bash
docker build -f Dockerfile.docker-desktop -t u6o-host:latest .
docker tag u6o-host:latest cduncc/u6o-host:latest
docker push cduncc/u6o-host:latest
```

*Note: For a purely local Docker Desktop cluster you can skip the tag/push and
set `imagePullPolicy: IfNotPresent` in `deployment.yaml` to use the locally
built image. The `push` step is for serving the image to a remote VM.*

### 2. Deploy to Kubernetes

Apply the manifests:

```powershell
kubectl apply -f k8s/docker-desktop/deployment.yaml
```

### 3. Verify the Deployment

Check the status of the pods:

```powershell
kubectl get pods -l app=u6o-host
```

*Note: The pod runs a Linux container (Debian-based). On Docker Desktop for Windows, the hostPath `C:\host` is automatically mapped to the container via the virtualization layer.*

Check the service to find the external IP/port:

```powershell
kubectl get service u6o-host-service
```

## Configuration

-   **Local Data Mount**: The deployment mounts `C:\host` from the host machine
    to `/u6o-host/` in the container, which is also the host process's
    **working directory**. The host reads all of its data with paths relative to
    that working directory, so every file lives on your local drive and can be
    edited without rebuilding the image. The Linux host binary (`u6o-host`) is
    baked into the image at `/usr/local/bin` (it is *not* written into the mount),
    so the mount stays pure game data. Ensure `C:\host` exists and contains
    `ultima6/`, `host/`, `dr/`, `dns.txt`, etc.
-   **Managing host files**: Because the binary and the data are decoupled, you
    manage the world on the machine/VM running the cluster — edit `motd.txt`,
    drop in save files under `save/`, adjust `dns.txt` — and just restart the pod
    (`kubectl rollout restart deployment/u6o-host`) to pick them up. No image
    rebuild required.
-   **Port**: The host is configured to listen on port `22` (must match `dns.txt`).
-   **Persistence**: Player data and world state are preserved in the `save/` directory within the `C:\host` mount.
-   **Logs**: You can view the server logs using:
    ```powershell
    kubectl logs -f deployment/u6o-host
    ```

## Troubleshooting

### Error: "no match for platform in manifest: not found"
This error occurs when you try to run an image for a platform not supported by your Docker configuration.
- **Solution**: Ensure Docker Desktop is in **Linux Containers** mode.

### Pod stays in "Pending"
- Ensure your Kubernetes node (typically a Linux VM in Docker Desktop) has enough resources.

### Pod starts, but the host logs "data mount … looks empty"
The pod is running, yet the host can't find `dr/ host/ ultima6/ dns.txt` and the
startup diagnostic says the mount looks empty — even though `C:\host` is full.

This means the `hostPath` in `deployment.yaml` points to a location the
**Kubernetes node** does *not* see your Windows folder, so an empty directory
got mounted instead. The node-internal path for the Windows `C:` drive differs
by Docker Desktop backend/version:

- **WSL2 backend** (modern default): `/run/desktop/mnt/host/c/host`
- **Legacy Hyper-V / Moby backend**: `/host_mnt/c/host`

Find the exact path **your** node uses with the bundled probe (it bind-mounts the
node root and reports which path actually holds your data):

```powershell
kubectl apply  -f k8s/docker-desktop/hostpath-probe.yaml
kubectl logs   hostpath-probe
kubectl delete -f k8s/docker-desktop/hostpath-probe.yaml
```

The probe prints a line like `^^^ FOUND DATA — set hostPath.path: /host_mnt/c/host ^^^`.
Put that value in `deployment.yaml` under `hostPath.path`, then
`kubectl apply -f k8s/docker-desktop/deployment.yaml` and
`kubectl rollout restart deployment/u6o-host`.

The manifest uses `type: Directory` (not `DirectoryOrCreate`) precisely so a
wrong path **fails loudly** instead of silently mounting an empty folder again.
If the probe finds nothing, your drive isn't shared into the node — for the
Hyper-V backend, enable it under **Settings → Resources → File Sharing** and
re-run the probe.

### Error: "failed to fulfil mount request ... no such file or directory"
The pod can't mount the `C:\host` volume even though the folder exists in Windows.

This is a **hostPath translation** issue, not a missing folder. The kubelet
evaluates `hostPath` *inside* the Kubernetes node (Docker Desktop's Linux VM),
where Windows paths like `C:/host` are meaningless, and with `type: Directory` a
path that doesn't exist on the node fails the mount. Use the probe above to find
the node's real path for `C:\host`, then set `hostPath.path` to it. Common values:

- **WSL2 backend (modern default)**: `/run/desktop/mnt/host/c/host`
- **Legacy Hyper-V backend**: `/host_mnt/c/host` (and share the drive under
  **Settings → Resources → File Sharing**)
- **Different drive/folder**: mirror the pattern, e.g. `D:\u6o\host` →
  `/run/desktop/mnt/host/d/u6o/host` (drive letter is lowercased).

### Files "not found" in logs
If the logs show `File .\dr\objfloat.flg not found (errno 2: No such file or directory)`:
1. **Check Directory Structure**: Ensure you didn't accidentally put a nested `assets` or `game_files` folder inside `C:\host`. The `dr/`, `host/`, and `ultima6/` folders must be in the root of `C:\host`.
2. **Docker Shared Folders**: On the **legacy Hyper-V backend**, ensure `C:\` (or the specific folder) is shared with Docker Desktop (Settings -> Resources -> File Sharing). The WSL2 backend shares all drives automatically.
3. **Internal Path**: The manifest mounts `/run/desktop/mnt/host/c/host` (the WSL2 node's view of `C:\host`). If your data still looks missing, your node likely exposes the drive elsewhere — run the probe (see *"Pod starts, but the host logs … looks empty"* above) and set `hostPath.path` to whatever path it reports.
4. **Permissions**: Ensure the files are not marked as "hidden" or "system" in Windows. The host now automatically falls back to read-only mode if a file exists but cannot be opened for writing.
