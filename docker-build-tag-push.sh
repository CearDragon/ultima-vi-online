#!/bin/bash
# LH-P7: Build and push the Linux host image.

# Exit on any error
set -e

echo "Building u6o-host:latest..."
docker build -t u6o-host:latest .

echo "Tagging..."
docker tag u6o-host:latest cduncc/u6o-host:latest

echo "Pushing..."
docker push cduncc/u6o-host:latest

echo "Restarting deployment..."
kubectl rollout restart deployment/u6o-host

echo "Done."