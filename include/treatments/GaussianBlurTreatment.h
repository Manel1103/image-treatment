#ifndef GAUSSIAN_BLUR_TREATMENT_H
#define GAUSSIAN_BLUR_TREATMENT_H

#include "../Treatment.h"
#include <sstream>

/**
 * @brief Applies Gaussian blur to an image
 * 
 * Gaussian blur is effective for smoothing images and reducing noise.
 * It uses a Gaussian kernel for convolution.
 */
class GaussianBlurTreatment : public Treatment {
private:
    int kernelSize;  // Must be odd and positive
    double sigmaX;   // Standard deviation in X direction
    double sigmaY;   // Standard deviation in Y direction

public:
    GaussianBlurTreatment(int kSize = 5, double sX = 0.0, double sY = 0.0) 
        : kernelSize(kSize), sigmaX(sX), sigmaY(sY) {
        // Ensure kernel size is odd and positive
        if (kernelSize % 2 == 0) kernelSize++;
        if (kernelSize < 1) kernelSize = 1;
    }

    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        cv::GaussianBlur(input, output, cv::Size(kernelSize, kernelSize), sigmaX, sigmaY);
        return output;
    }

    std::string getName() const override {
        return "Gaussian Blur";
    }

    std::string getDescription() const override {
        return "Applies Gaussian blur to smooth images and reduce noise";
    }

    std::map<std::string, std::string> getParameters() const override {
        std::map<std::string, std::string> params;
        params["kernelSize"] = std::to_string(kernelSize);
        params["sigmaX"] = std::to_string(sigmaX);
        params["sigmaY"] = std::to_string(sigmaY);
        return params;
    }

    bool setParameter(const std::string& paramName, const std::string& value) override {
        try {
            if (paramName == "kernelSize") {
                int val = std::stoi(value);
                if (val > 0) {
                    kernelSize = (val % 2 == 0) ? val + 1 : val;  // Ensure odd
                    return true;
                }
            } else if (paramName == "sigmaX") {
                sigmaX = std::stod(value);
                return true;
            } else if (paramName == "sigmaY") {
                sigmaY = std::stod(value);
                return true;
            }
        } catch (...) {
            return false;
        }
        return false;
    }

    std::map<std::string, std::string> getParameterInfo() const override {
        std::map<std::string, std::string> info;
        info["kernelSize"] = "int (odd, positive) - Size of the Gaussian kernel";
        info["sigmaX"] = "double - Standard deviation in X direction (0 = auto)";
        info["sigmaY"] = "double - Standard deviation in Y direction (0 = auto)";
        return info;
    }

    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<GaussianBlurTreatment>(kernelSize, sigmaX, sigmaY);
    }
};

#endif // GAUSSIAN_BLUR_TREATMENT_H

