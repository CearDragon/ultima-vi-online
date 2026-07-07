#!/bin/bash
# LH-P7: Build and push the Linux host image for AKS.

set -euo pipefail

echo "Building acrultimavionline.azurecr.io/u6o-host:latest using k8s/aks/Dockerfile..."
docker build \
  -f "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/Dockerfile" \
  -t "acrultimavionline.azurecr.io/u6o-host:latest" \
  "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../.."

echo "Pushing acrultimavionline.azurecr.io/u6o-host:latest..."
docker push acrultimavionline.azurecr.io/u6o-host:latest

echo "Restarting deployment u6o-host in namespace ultima-dev..."
kubectl rollout restart deployment/u6o-host -n ultima-dev

echo "Waiting for rollout to complete..."
kubectl rollout status deployment/u6o-host -n ultima-dev

echo "Done."