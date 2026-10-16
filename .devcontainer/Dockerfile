FROM ubuntu:24.04

# Install required packages and development tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    python3-venv \
    python3-full \
    pkg-config \
    libgl-dev \
    libgl1-mesa-dev \
    libx11-xcb-dev \
    libfontenc-dev \
    libice-dev \
    libsm-dev \
    libxaw7-dev \
    libxcomposite-dev \
    libxcursor-dev \
    libxdamage-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxinerama-dev \
    libxkbfile-dev \
    libxmu-dev \
    libxmuu-dev \
    libxpm-dev \
    libxrandr-dev \
    libxrender-dev \
    libxres-dev \
    libxss-dev \
    libxt-dev \
    libxtst-dev \
    libxv-dev \
    libxxf86vm-dev \
    libxcb-glx0-dev \
    libxcb-render0-dev \
    libxcb-render-util0-dev \
    libxcb-xkb-dev \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-keysyms1-dev \
    libxcb-randr0-dev \
    libxcb-shape0-dev \
    libxcb-sync-dev \
    libxcb-xfixes0-dev \
    libxcb-xinerama0-dev \
    libxcb-dri3-dev \
    uuid-dev \
    libxcb-cursor-dev \
    libxcb-dri2-0-dev \
    libxcb-present-dev \
    libxcb-composite0-dev \
    libxcb-ewmh-dev \
    libxcb-res0-dev \
    libxcb-util-dev \
    libxcb-util0-dev \
    # Additional development tools
    git \
    gdb \
    valgrind \
    ccls \
    vim \
    curl \
    wget \
    htop \
    strace \
    ltrace \
    && rm -rf /var/lib/apt/lists/*

# Create a Python virtual environment and install Conan
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"
RUN pip install conan

# Create a working directory
WORKDIR /app

# We'll copy the source during development through the bind mount
# This keeps the Dockerfile as is, and the source will be mounted at runtime

# Configure environment with PATH for Conan
ENV PATH="/opt/venv/bin:$PATH"

# Set some environment variables to improve debugging
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

# Pre-create build directory
RUN mkdir -p /app/build

# Default command to keep container running
CMD ["sleep", "infinity"] 