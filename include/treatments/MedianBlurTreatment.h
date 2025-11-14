#ifndef MEDIAN_BLUR_TREATMENT_H
#define MEDIAN_BLUR_TREATMENT_H

#include "../Treatment.h"
#include <sstream>

/**
 * @brief Applies median blur to an image
 * 
 * Median blur is particularly effective at removing salt-and-pepper noise
 * while preserving edges better than Gaussian blur.
 */
class MedianBlurTreatment : public Treatment {
private:
    int kernelSize;  // Must be odd and positive

public:
    MedianBlurTreatment(int kSize = 5) : kernelSize(kSize) {
        // Ensure kernel size is odd and positive
        if (kernelSize % 2 == 0) kernelSize++;
        if (kernelSize < 1) kernelSize = 1;
    }

    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        cv::medianBlur(input, output, kernelSize);
        return output;
    }

    std::string getName() const override {
        return "Median Blur";
    }

    std::string getDescription() const override {
        return "Applies median filter to reduce salt-and-pepper noise";
    }

    std::map<std::string, std::string> getParameters() const override {
        std::map<std::string, std::string> params;
        params["kernelSize"] = std::to_string(kernelSize);
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
            }
        } catch (...) {
            return false;
        }
        return false;
    }

    std::map<std::string, std::string> getParameterInfo() const override {
        std::map<std::string, std::string> info;
        info["kernelSize"] = "int (odd, positive) - Size of the median filter kernel";
        return info;
    }

    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<MedianBlurTreatment>(kernelSize);
    }
};

#endif // MEDIAN_BLUR_TREATMENT_H








