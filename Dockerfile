# Stage 1: Build the C++ extension
FROM python:3.13.3-slim AS build

# Install system packages for C++/Python development
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-dev \
    python3-pybind11 \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies
COPY requirements.txt .
RUN pip install --upgrade pip && pip install -r requirements.txt

# Copy the source code
WORKDIR /app
COPY . .

# Build the C++ extension using CMake
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    cmake --build .

# Stage 2: Runtime container
FROM python:3.13.3-slim AS runtime

WORKDIR /app

# Copy just requirements first
COPY requirements.txt .

# Install dependencies here!
RUN pip install --no-cache-dir -r requirements.txt

# Now copy built app
COPY --from=build /app /app

ENTRYPOINT ["python3", "Main.py"]

