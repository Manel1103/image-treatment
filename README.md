# Image Treatment System

A flexible and extensible C++ image processing framework built with OpenCV.

## Overview

The Image Treatment System provides a modular architecture for applying various image processing operations (treatments) to images. It supports chaining multiple treatments together and includes various image sources.

## Features

- **Modular Architecture**: Base classes for treatments and image sources with easy extensibility
- **Treatment Chaining**: Apply multiple treatments in sequence with automatic intermediate result storage
- **Parameter Control**: Dynamic parameter modification for all treatments
- **Multiple Image Sources**: Support for file-based and webcam-based image input
- **10 Built-in Treatments**: Ready-to-use image processing operations

## Built-in Treatments

1. **GaussianBlurTreatment** - Gaussian blur for smoothing and noise reduction
2. **MedianBlurTreatment** - Median filter for salt-and-pepper noise removal
3. **CannyEdgeTreatment** - Canny edge detection algorithm
4. **ThresholdTreatment** - Image thresholding for binary image creation
5. **BrightnessTreatment** - Brightness and contrast adjustment
6. **GrayscaleTreatment** - Color to grayscale conversion
7. **SharpenTreatment** - Image sharpening for detail enhancement
8. **ErosionTreatment** - Morphological erosion operation
9. **DilationTreatment** - Morphological dilation operation
10. **MosaicTreatment** - Mosaic/pixelation effect

## Requirements

- CMake 3.10 or higher
- C++17 compatible compiler
- OpenCV 4.x or higher

## Building

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Install (optional)
cmake --install .
```

## Usage

### Basic Example

```cpp
#include "ImageSource.h"
#include "TreatmentChain.h"
#include "treatments/GaussianBlurTreatment.h"
#include "treatments/CannyEdgeTreatment.h"

int main() {
    // Load an image
    FileImageSource source("input.jpg");
    cv::Mat image = source.getImage();
    
    // Create a treatment chain
    TreatmentChain chain;
    chain.addTreatment(std::make_unique<GaussianBlurTreatment>(5));
    chain.addTreatment(std::make_unique<CannyEdgeTreatment>(50, 150));
    
    // Process the image
    cv::Mat result = chain.processChain(image);
    
    // Save or display result
    cv::imwrite("output.jpg", result);
    
    return 0;
}
```

### Running the Program

```bash
# Run the interactive menu
./image_treatment
```

The program provides an interactive menu to:
- Test webcam capture
- Test webcam with image treatments
- Load and process images from files
- Chain multiple treatments together
- View intermediate processing results

## Architecture

### Core Classes

#### `Treatment` (Abstract Base Class)
All image treatments inherit from this class and implement:
- `process()` - Apply the treatment to an image
- `getName()` - Get treatment name
- `getDescription()` - Get treatment description
- `getParameters()` / `setParameter()` - Parameter management
- `clone()` - Create a copy of the treatment

#### `TreatmentChain`
Manages a sequence of treatments:
- `addTreatment()` - Add treatment to end of chain
- `insertTreatment()` - Insert treatment at specific position
- `removeTreatment()` - Remove treatment from chain
- `processChain()` - Process image through all treatments
- `getIntermediateResult()` - Access intermediate results

#### `ImageSource` (Abstract Base Class)
Defines interface for image sources:
- `FileImageSource` - Load images from files
- `WebcamImageSource` - Capture from webcam/camera

## Creating Custom Treatments

To create a custom treatment:

```cpp
#include "Treatment.h"

class MyTreatment : public Treatment {
private:
    int myParameter;

public:
    MyTreatment(int param = 10) : myParameter(param) {}
    
    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        // Your processing logic here
        return output;
    }
    
    std::string getName() const override {
        return "My Treatment";
    }
    
    std::string getDescription() const override {
        return "Description of what this treatment does";
    }
    
    // Implement other required methods...
};
```

## Project Structure

```
ImageTreatmentSystem/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── ImageSource.h
│   ├── Treatment.h
│   ├── TreatmentChain.h
│   └── treatments/
│       ├── GaussianBlurTreatment.h
│       ├── CannyEdgeTreatment.h
│       ├── ThresholdTreatment.h
│       ├── BrightnessTreatment.h
│       ├── MedianBlurTreatment.h
│       ├── GrayscaleTreatment.h
│       ├── SharpenTreatment.h
│       ├── ErosionTreatment.h
│       ├── DilationTreatment.h
│       └── MosaicTreatment.h
└── src/
    └── test_webcam.cpp
```

## License

This project is provided as-is for educational and commercial purposes.

## Contributing

To add new treatments:
1. Create a new header file in `include/treatments/`
2. Inherit from the `Treatment` base class
3. Implement all required virtual methods
4. Add the header to `CMakeLists.txt`

## Interactive Features

The program provides an interactive menu system that allows you to:
- Test webcam capture with different device IDs
- Apply predefined treatment chains to webcam images
- Load images from files and apply custom treatment chains
- View intermediate results at each processing step
- Save processed images with timestamps

All treatments can be chained together in any order, and you can view the results at each step of the processing pipeline.

