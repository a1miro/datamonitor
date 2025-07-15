# Real-Time Data Monitor

A high-performance real-time data plotting application built with Qt 6 and QML Scene Graph for 60 FPS rendering.

## Features

- **Real-time Data Generation**: Sine wave data generated every 1ms (1000 Hz)
- **High-Performance Rendering**: 60 FPS rendering using QML Scene Graph
- **Interactive Controls**: 
  - Start/Stop data generation
  - Adjustable frequency (0.1 - 10 Hz)
  - Adjustable amplitude (0.1 - 5.0)
  - Time window control (1 - 10 seconds)
- **Auto-scaling**: Automatic Y-axis scaling based on visible data
- **Thread-safe**: Data generation runs in background with proper synchronization

## Architecture

### C++ Components

1. **DataGenerator**: 
   - Generates sine wave data points every 1ms using QTimer
   - Thread-safe data buffer with mutex protection
   - Configurable frequency, amplitude, and buffer size

2. **PlotRenderer**: 
   - QQuickItem subclass for QML integration
   - Manages data updates and triggers Scene Graph updates at 60 FPS
   - Handles auto-scaling and time window management

3. **PlotNode**: 
   - QSGGeometryNode for efficient GPU rendering
   - Line strip geometry for smooth curve rendering
   - Optimized vertex buffer management

### QML Components

1. **main.qml**: Main application window with controls
2. **PlotView.qml**: Plot visualization with grid and labels

## Building

### Prerequisites

- Qt 6.2 or later
- CMake 3.16 or later
- C++17 compatible compiler

### Build Steps

```bash
mkdir build
cd build
cmake ..
make
```

### Running

```bash
./datamonitor
```

## Performance Characteristics

- **Data Generation**: 1000 Hz (1 point per millisecond)
- **Rendering Rate**: 60 FPS
- **Data Buffer**: Configurable size (default: 5000 points = 5 seconds)
- **Memory Usage**: Optimized with circular buffer and Scene Graph caching

## Technical Details

### Scene Graph Optimization

- Uses QSGGeometry with line strip primitive for efficient rendering
- Vertex buffer reuse to minimize GPU memory allocations
- Automatic geometry updates only when needed
- Hardware-accelerated rendering through OpenGL

### Threading Model

- Data generation runs on main thread using QTimer
- Scene Graph updates run on dedicated render thread
- Thread-safe data access using QMutex
- Non-blocking data retrieval for UI updates

### Memory Management

- Circular buffer prevents unbounded memory growth
- Automatic cleanup of old data points
- Efficient vertex buffer allocation/reallocation
- Scene Graph node reuse

## Customization

### Changing Data Generation

Modify `DataGenerator::generateDataPoint()` to implement different wave forms:

```cpp
// Square wave
double y = m_amplitude * (qSin(2.0 * M_PI * m_frequency * m_time) > 0 ? 1 : -1);

// Sawtooth wave
double phase = fmod(m_frequency * m_time, 1.0);
double y = m_amplitude * (2.0 * phase - 1.0);
```

### Adjusting Render Performance

- Modify update timer interval in `PlotRenderer` constructor
- Change buffer size for different time windows
- Adjust line width for different visual styles

## License

This project is provided as an example implementation of real-time data plotting with Qt Scene Graph.
