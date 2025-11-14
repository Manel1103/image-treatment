#ifndef IMAGE_SOURCE_H
#define IMAGE_SOURCE_H

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

/**
 * @brief Abstract base class for image sources
 */
class ImageSource {
public:
    virtual ~ImageSource() = default;

    /**
     * @brief Get an image from this source
     * @return The captured/loaded image
     */
    virtual cv::Mat getImage() = 0;

    /**
     * @brief Check if the source is available
     * @return true if source is ready, false otherwise
     */
    virtual bool isAvailable() const = 0;

    /**
     * @brief Get a description of this source
     * @return Description string
     */
    virtual std::string getDescription() const = 0;
};

/**
 * @brief Image source from a file
 */
class FileImageSource : public ImageSource {
private:
    std::string filepath;
    cv::Mat image;

public:
    explicit FileImageSource(const std::string& path) : filepath(path) {
        image = cv::imread(filepath);
    }

    cv::Mat getImage() override {
        return image.clone();
    }

    bool isAvailable() const override {
        return !image.empty();
    }

    std::string getDescription() const override {
        return "File: " + filepath;
    }
};

/**
 * @brief Image source from a webcam
 */
class WebcamImageSource : public ImageSource {
private:
    cv::VideoCapture capture;
    int deviceId;

public:
    explicit WebcamImageSource(int id = 0) : deviceId(id) {
        capture.open(deviceId);
    }

    ~WebcamImageSource() {
        if (capture.isOpened()) {
            capture.release();
        }
    }

    cv::Mat getImage() override {
        cv::Mat frame;
        if (capture.isOpened()) {
            // Try to read a frame
            capture >> frame;
            
            // If frame is empty, try a few more times (common with all camera types)
            if (frame.empty()) {
                for (int attempt = 0; attempt < 3; attempt++) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    capture >> frame;
                    if (!frame.empty()) {
                        break;
                    }
                }
            }
        }
        return frame;
    }

    /**
     * @brief Capture a frame with retry and validation (works with any camera source)
     * This method is more robust and handles initialization delays common with:
     * - Virtual webcams (Camo Studio, OBS Virtual Camera, etc.)
     * - USB webcams that need time to initialize
     * - Network cameras with latency
     * 
     * @param skipFrames Number of frames to skip before capturing (default: 5)
     * @param retries Number of retry attempts (default: 15)
     * @param validateNonBlack If true, skip frames that are entirely black (default: true)
     * @return The captured frame
     */
    cv::Mat getImageWithRetry(int skipFrames = 5, int retries = 15, bool validateNonBlack = true) {
        cv::Mat frame;
        if (!capture.isOpened()) {
            return frame;
        }
        
        // Skip initial frames (many cameras need a few frames to initialize properly)
        // This works for both physical and virtual cameras
        for (int i = 0; i < skipFrames; i++) {
            capture >> frame;
            // Small delay to let the camera stabilize
            if (i < skipFrames - 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        }
        
        // Try to capture a valid frame
        for (int attempt = 0; attempt < retries; attempt++) {
            capture >> frame;
            
            if (!frame.empty()) {
                // If validation is enabled, check if frame is not entirely black
                if (validateNonBlack) {
                    cv::Scalar mean = cv::mean(frame);
                    // Check if any channel has significant content (threshold: 5)
                    // This works for both color and grayscale images
                    double maxMean = std::max({mean[0], mean[1], mean[2]});
                    
                    if (maxMean > 5.0) {
                        // Valid non-black frame found
                        return frame;
                    }
                    // Frame is black, continue trying
                } else {
                    // No validation, return first non-empty frame
                    return frame;
                }
            }
            
            // Small delay between attempts to avoid overwhelming the camera
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        // If we get here, either all frames were black or empty
        // Return the last frame we got (even if black/empty) so caller can handle it
        return frame;
    }
    
    /**
     * @brief Get a stable frame (waits for camera to stabilize)
     * This is a convenience method that uses sensible defaults for most cameras
     * @return The captured frame
     */
    cv::Mat getStableImage() {
        return getImageWithRetry(8, 20, true);
    }

    bool isAvailable() const override {
        return capture.isOpened();
    }

    std::string getDescription() const override {
        return "Webcam (device " + std::to_string(deviceId) + ")";
    }

    /**
     * @brief Capture a single frame and freeze it
     * @return The captured frame
     */
    cv::Mat captureFrame() {
        cv::Mat frame = getImage();
        if (capture.isOpened()) {
            capture.release();
        }
        return frame;
    }
};

#endif // IMAGE_SOURCE_H

