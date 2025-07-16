# Stage 1: Build the C++ extension
FROM python:3.13.3-slim AS build

ENV PYTHON_EXECUTABLE=/usr/local/bin/python3
ENV PYTHON_INCLUDE_DIR=/usr/local/include/python3.13

# Install system packages for C++/Python development
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libgmock-dev \
    libgtest-dev \
    libpython3-dev \
    pybind11-dev \
    python3-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies
COPY requirements.txt .
RUN pip install --upgrade pip && pip install -r requirements.txt pybind11

# Copy the source code
WORKDIR /app
COPY . .

# ИСПРАВЛЕНИЕ: Очищаем старый CMake кэш и собираем заново
RUN rm -rf build CMakeCache.txt && \
    mkdir -p build && \
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

# Запускаем тесты для проверки сборки
RUN cd /app && ./build/Test

ENTRYPOINT ["python3", "Main.py"]