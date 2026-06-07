# Running Ultima VI Online Host in Kubernetes

This directory contains the necessary files to run the Ultima VI Online Host in a local Kubernetes cluster (Docker Desktop).

## Prerequisites

1.  **Docker Desktop** with **Windows Containers** enabled.
    - Right-click the Docker icon in the tray and select "Switch to Windows containers..." if it's not already enabled.
2.  **Kubernetes** enabled in Docker Desktop settings.
3.  **Build the Host**: The Dockerfile expects the host to be built in Release mode.
    - Ensure `bin\host\release\Ultima VI Online Host.exe` exists.
4.  **Map Data**: The `ultima6` folder must exist at the project root and contain the original Ultima 6 map files (`chunks`, `map`, etc.).

## Deployment Steps

### 1. Build the Docker Image

Run the following command from the project root:

```powershell
docker build -t u6o7-host:latest .
```

*Note: The Dockerfile in the root is used by default.*

### 2. Deploy to Kubernetes

Apply the manifests:

```powershell
kubectl apply -f k8s/deployment.yaml
```

### 3. Verify the Deployment

Check the status of the pods:

```powershell
kubectl get pods -l app=u6o7-host
```

*Note: Windows pods may take several minutes to start as the base image is large.*

Check the service to find the external IP/port:

```powershell
kubectl get service u6o7-host-service
```

## Configuration

-   **Port**: The host is configured to listen on port `2593` (defined in `dns.txt` during build).
-   **Persistence**: The `save` directory is mounted as a PersistentVolumeClaim to ensure player data and world state are preserved across pod restarts.
-   **Logs**: You can view the server logs using:
    ```powershell
    kubectl logs -f deployment/u6o7-host
    ```

## Troubleshooting

### Error: "no match for platform in manifest: not found"
This error occurs when Docker Desktop is set to **Linux Containers** mode but you are trying to build/pull a Windows image.
- **Solution**: Right-click the Docker tray icon and select **"Switch to Windows containers..."**.

### Pod stays in "Pending"
- Ensure you have Windows nodes available. In Docker Desktop, the single node should support both Linux and Windows containers if configured correctly, but Windows containers require "Switch to Windows containers" to be active.
-   If map data is missing, the host will fail to start. Ensure the `ultima6` folder is populated before building the image.
