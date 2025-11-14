#ifndef THRESHOLD_TREATMENT_H
#define THRESHOLD_TREATMENT_H

#include "../Treatment.h"
#include <sstream>

/**
 * @brief Applies thresholding to an image
 * 
 * Thresholding is used to create binary images from grayscale images.
 * Pixels above the threshold become white, below become black (or vice versa).
 */
class ThresholdTreatment : public Treatment {
private:
    double thresholdValue;  // Threshold value
    double maxValue;        // Maximum value to use with THRESH_BINARY and THRESH_BINARY_INV
    int thresholdType;      // Type of thresholding (cv::ThresholdTypes)

public:
    ThresholdTreatment(double thresh = 127.0, double maxVal = 255.0, int type = cv::THRESH_BINARY)
        : thresholdValue(thresh), maxValue(maxVal), thresholdType(type) {}

    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        cv::Mat gray;
        
        // Convert to grayscale if needed
        if (input.channels() == 3) {
            cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = input;
        }
        
        cv::threshold(gray, output, thresholdValue, maxValue, thresholdType);
        return output;
    }

    std::string getName() const override {
        return "Threshold";
    }

    std::string getDescription() const override {
        return "Applies thresholding to create binary images";
    }

    std::map<std::string, std::string> getParameters() const override {
        std::map<std::string, std::string> params;
        params["thresholdValue"] = std::to_string(thresholdValue);
        params["maxValue"] = std::to_string(maxValue);
        params["thresholdType"] = std::to_string(thresholdType);
        return params;
    }

    bool setParameter(const std::string& paramName, const std::string& value) override {
        try {
            if (paramName == "thresholdValue") {
                thresholdValue = std::stod(value);
                return true;
            } else if (paramName == "maxValue") {
                maxValue = std::stod(value);
                return true;
            } else if (paramName == "thresholdType") {
                int val = std::stoi(value);
                if (val >= 0 && val <= 4) {
                    thresholdType = val;
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
        info["thresholdValue"] = "double - Threshold value";
        info["maxValue"] = "double - Maximum value for binary modes";
        info["thresholdType"] = "int (0-4) - 0:BINARY, 1:BINARY_INV, 2:TRUNC, 3:TOZERO, 4:TOZERO_INV";
        return info;
    }

    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<ThresholdTreatment>(thresholdValue, maxValue, thresholdType);
    }

    bool validateInput(const cv::Mat& input) const override {
        return !input.empty() && (input.channels() == 1 || input.channels() == 3);
    }
};

#endif // THRESHOLD_TREATMENT_H








