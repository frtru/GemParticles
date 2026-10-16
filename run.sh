#!/bin/bash

# Exit on error
set -e

cd "$(dirname "$0")"

# Enable X11 access from the Docker container
echo "Enabling X11 access for Docker..."
xhost +local:docker

echo "Starting GemParticles..."
sudo docker compose -f docker/docker-compose.yml up

# Disable X11 access when done
echo "Disabling X11 access..."
xhost -local:docker

echo "Application exited." 