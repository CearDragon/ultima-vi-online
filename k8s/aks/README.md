# Running Ultima VI Online Host in Kubernetes

This directory contains deployment manifests and a dedicated Linux-host Dockerfile for running the Ultima VI Online Host in a Kubernetes cluster (AKS).

## Prerequisites

1.  **Docker** with Linux container support.
2.  **kubectl** configured to access your AKS cluster.
3.  **Azure Container Registry (ACR)** access (e.g., `acrultimavionline.azurecr.io`).
4.  **PersistentVolumeClaim** provisioned with your cluster (via `pvc.yaml`).
5.  **Azure Storage Account credentials** — your storage account key (see **Configuration** below).

## Architecture

-   **Linux i386 host**: The `Dockerfile` in this directory builds the headless host as a 32-bit Linux executable and runs it from `/host` (the mounted volume).
-   **No bundled game data in image**: All runtime data (chunks, map, saves, NPC flags, etc.) is expected to be provisioned at the mounted PVC at deploy time.
-   **Execution from /host**: The container's working directory is `/host`, matching the Kubernetes volume mount in `deployment.yaml`.

## Deployment Steps

### 1. Build the Docker Image Locally

From the project root, build the Linux-host image using the AKS Dockerfile:

```powershell
docker build -f k8s/aks/Dockerfile -t u6o-host:latest .
docker tag u6o-host:latest acrultimavionline.azurecr.io/u6o-host:latest
```

### 2. Push to Azure Container Registry

Authenticate and push the image:

```powershell
az acr login --name acrultimavionline
docker push acrultimavionline.azurecr.io/u6o-host:latest
```

### 3. Deploy to AKS

Apply the full deployment stack (namespace, PV, PVC, deployment, service):

```powershell
kubectl apply -f k8s/aks/namespace.yaml
kubectl apply -f k8s/aks/pv.yaml
kubectl apply -f k8s/aks/pvc.yaml
kubectl apply -f k8s/aks/deployment.yaml
```

Or apply all at once:

```powershell
kubectl apply -f k8s/aks/
```

### 4. Verify the Deployment

Check pod status:

```powershell
kubectl get pods -l app=u6o-host -n ultima-dev
```

View pod logs:

```powershell
kubectl logs -f deployment/u6o-host -n ultima-dev
```

Check the LoadBalancer service for the external endpoint:

```powershell
kubectl get service u6o-host-service -n ultima-dev
```

The external IP and port will be listed under `EXTERNAL-IP:PORT`. Connect on port 22 to reach the host.

### 5. Restart the Deployment

To restart the host pod (e.g., after updating the image or configuration):

```powershell
kubectl rollout restart deployment/u6o-host -n ultima-dev
```

Monitor the rollout:

```powershell
kubectl rollout status deployment/u6o-host -n ultima-dev
```

## Configuration

-   **Port**: The host listens on port `22` (defined in `deployment.yaml` and `dns.txt` in the runtime image).
-   **Working Directory**: The container's working directory is `/host`, which is mounted to a PersistentVolumeClaim. Game data, saves, and configuration must be provisioned at this path.
-   **Graceful Shutdown**: The deployment specifies `terminationGracePeriodSeconds: 60` to allow the host time to save world state before forceful termination (SIGTERM → graceful exit, then SIGKILL if needed).
-   **Image Pull Policy**: Set to `Always` to ensure the latest image is used on each deployment update.

### Configuring Azure Storage Secrets

The `secrets.yaml` file **is not committed to the repository** for security reasons. You must configure it locally before deployment:

1.  Copy `secrets.yaml` to your local `k8s/aks/` directory (it is gitignored).
2.  Replace `<YOUR_AZURE_STORAGE_ACCOUNT_KEY>` with your actual Azure Storage Account key:
    ```yaml
    stringData:
      azurestorageaccountname: stultimasixonline
      azurestorageaccountkey: "YOUR_ACTUAL_KEY_HERE"
    ```
3.  Do **NOT** commit this file to git. The `.gitignore` prevents accidental commits.
4.  When deploying, ensure `secrets.yaml` exists in the `k8s/aks/` directory.

To retrieve your storage account key from Azure:

```powershell
az storage account keys list --resource-group <your-rg> --account-name stultimasixonline
```

Retrieve the value of `value` from the first key (key1) and paste it into `secrets.yaml`.

## Provisioning Runtime Data

The image **does not bundle game data**. Before the pod starts successfully, you must provision:

1.  **chunks**, **map**, and other game files from the original Ultima 6 (into the PVC)
2.  **host/** directory with NPC and flag data
3.  **save/** directory (for player saves and world state)
4.  **dns.txt** (or let the Dockerfile's default `localhost:22` stand)

These should all be present at the PVC mount point (`/host` inside the container) at deploy time.

## Troubleshooting

### Pod stays in "Pending" or "CrashLoopBackOff"

-   **Missing game data**: Ensure all required game files are in the PVC before the pod starts. The host will exit if it cannot find `ultima6/chunks`, `ultima6/map`, `host/`, etc.
    ```powershell
    kubectl exec -it deployment/u6o-host -n ultima-dev -- ls -la /host
    ```
    Check that `chunks`, `map`, `host/`, and `save/` directories exist.

-   **PVC not provisioned**: Verify the PersistentVolumeClaim is bound:
    ```powershell
    kubectl get pvc -n ultima-dev
    ```

-   **Image pull failures**: Ensure ACR credentials are available and the image tag is correct:
    ```powershell
    kubectl describe pod -l app=u6o-host -n ultima-dev
    ```

### Host crashes after startup

-   **Check logs** for missing data or initialization errors:
    ```powershell
    kubectl logs deployment/u6o-host -n ultima-dev --previous
    ```
-   **Verify SIGTERM handling**: The host should gracefully save state on pod deletion. If the pod is force-killed, check `save/` for corruption.

## Advanced: Building for a Different Registry

To build and push to a different ACR or registry:

```powershell
$REGISTRY = "myregistry.azurecr.io"
docker build -f k8s/aks/Dockerfile -t u6o-host:latest .
docker tag u6o-host:latest "$REGISTRY/u6o-host:latest"
az acr login --name myregistry
docker push "$REGISTRY/u6o-host:latest"
```

Then update the `image:` field in `deployment.yaml` to match `$REGISTRY/u6o-host:latest`.
