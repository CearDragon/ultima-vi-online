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

Run the following command from the project root:

```bash
docker build -t u6o-host:latest .
docker tag u6o-host:latest cduncc/u6o-host:latest
docker push cduncc/u6o-host:latest
```

*Note: The Dockerfile in the root is used by default.*

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

-   **Local Data Mount**: The deployment is configured to mount `C:\host` from
    the host machine to `/u6o-host/` in the container. This allows loading
    game data from your local drive. The Linux host binary (`u6o-host`) is
    built into the Docker image and copied into `/u6o-host/` at startup to
    run alongside the data. Ensure `C:\host` exists and contains `ultima6/`,
    `host/`, `dr/`, `dns.txt`, etc.
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

### Files "not found" in logs
If the logs show `File .\dr\objfloat.flg not found (errno 2: No such file or directory)`:
1. **Check Directory Structure**: Ensure you didn't accidentally put a nested `assets` or `game_files` folder inside `C:\host`. The `dr/`, `host/`, and `ultima6/` folders must be in the root of `C:\host`.
2. **Docker Shared Folders**: Ensure `C:\` (or the specific folder) is shared with Docker Desktop (Settings -> Resources -> File Sharing).
3. **Internal Path**: In some Docker Desktop versions, you may need to edit `deployment.yaml` and change `path: C:/host` to `/run/desktop/mnt/host/c/host`.
4. **Permissions**: Ensure the files are not marked as "hidden" or "system" in Windows. The host now automatically falls back to read-only mode if a file exists but cannot be opened for writing.
