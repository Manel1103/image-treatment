#ifndef EROSION_TREATMENT_H
#define EROSION_TREATMENT_H

#include "../Treatment.h"

/**
 * @brief Applies morphological erosion to an image
 * 
 * Erosion erodes away the boundaries of foreground objects.
 * It's useful for removing small white noise, separating objects, etc.
 */
class ErosionTreatment : public Treatment {
private:
    int kernelSize;    // Size of the structuring element
    int kernelShape;   // Shape: 0=RECT, 1=CROSS, 2=ELLIPSE
    int iterations;    // Number of times erosion is applied

public:
    ErosionTreatment(int kSize = 3, int shape = cv::MORPH_RECT, int iter = 1)
        : kernelSize(kSize), kernelShape(shape), iterations(iter) {
        if (kernelSize < 1) kernelSize = 1;
        if (iterations < 1) iterations = 1;
    }

    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        cv::Mat element = cv::getStructuringElement(
            kernelShape,
            cv::Size(kernelSize, kernelSize)
        );
        cv::erode(input, output, element, cv::Point(-1, -1), iterations);
        return output;
    }

    std::string getName() const override {
        return "Erosion";
    }

    std::string getDescription() const override {
        return "Erodes boundaries of foreground objects";
    }

    std::map<std::string, std::string> getParameters() const override {
        std::map<std::string, std::string> params;
        params["kernelSize"] = std::to_string(kernelSize);
        params["kernelShape"] = std::to_string(kernelShape);
        params["iterations"] = std::to_string(iterations);
        return params;
    }

    bool setParameter(const std::string& paramName, const std::string& value) override {
        try {
            if (paramName == "kernelSize") {
                int val = std::stoi(value);
                if (val > 0) {
                    kernelSize = val;
                    return true;
                }
            } else if (paramName == "kernelShape") {
                int val = std::stoi(value);
                if (val >= 0 && val <= 2) {
                    kernelShape = val;
                    return true;
                }
            } else if (paramName == "iterations") {
                int val = std::stoi(value);
                if (val > 0) {
                    iterations = val;
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
        info["kernelSize"] = "int - Size of structuring element";
        info["kernelShape"] = "int - Shape: 0=RECT, 1=CROSS, 2=ELLIPSE";
        info["iterations"] = "int - Number of erosion iterations";
        return info;
    }

    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<ErosionTreatment>(kernelSize, kernelShape, iterations);
    }
};

#endif // EROSION_TREATMENT_H








