#ifndef GRAYSCALE_TREATMENT_H
#define GRAYSCALE_TREATMENT_H

#include "../Treatment.h"

/**
 * @brief Converts a color image to grayscale
 * 
 * Converts BGR color images to grayscale using OpenCV's standard conversion.
 * If the input is already grayscale, it returns a copy.
 */
class GrayscaleTreatment : public Treatment {
public:
    GrayscaleTreatment() = default;

    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        
        if (input.channels() == 3) {
            cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
        } else if (input.channels() == 4) {
            cv::cvtColor(input, output, cv::COLOR_BGRA2GRAY);
        } else {
            // Already grayscale
            output = input.clone();
        }
        
        return output;
    }

    std::string getName() const override {
        return "Grayscale";
    }

    std::string getDescription() const override {
        return "Converts color image to grayscale";
    }

    std::map<std::string, std::string> getParameters() const override {
        return std::map<std::string, std::string>();  // No parameters
    }

    bool setParameter(const std::string& paramName, const std::string& value) override {
        return false;  // No parameters to set
    }

    std::map<std::string, std::string> getParameterInfo() const override {
        return std::map<std::string, std::string>();  // No parameters
    }

    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<GrayscaleTreatment>();
    }
};

#endif // GRAYSCALE_TREATMENT_H








