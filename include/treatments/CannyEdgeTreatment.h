#ifndef CANNY_EDGE_TREATMENT_H
#define CANNY_EDGE_TREATMENT_H

#include "../Treatment.h"
#include <sstream>

/**
 * @brief Applies Canny edge detection to an image
 * 
 * The Canny edge detector is a multi-stage algorithm for detecting
 * edges in images. It produces a binary image showing the detected edges.
 */
class CannyEdgeTreatment : public Treatment {
private:
    double threshold1;  // First threshold for the hysteresis procedure
    double threshold2;  // Second threshold for the hysteresis procedure
    int apertureSize;   // Aperture size for the Sobel operator (must be 3, 5, or 7)

public:
    CannyEdgeTreatment(double t1 = 50.0, double t2 = 150.0, int aperture = 3)
        : threshold1(t1), threshold2(t2), apertureSize(aperture) {
        // Ensure aperture size is valid (3, 5, or 7)
        if (apertureSize != 3 && apertureSize != 5 && apertureSize != 7) {
            apertureSize = 3;
        }
    }

    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        cv::Mat gray;
        
        // Convert to grayscale if needed
        if (input.channels() == 3) {
            cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = input;
        }
        
        cv::Canny(gray, output, threshold1, threshold2, apertureSize);
        return output;
    }

    std::string getName() const override {
        return "Canny Edge Detection";
    }

    std::string getDescription() const override {
        return "Detects edges in images using the Canny algorithm";
    }

    std::map<std::string, std::string> getParameters() const override {
        std::map<std::string, std::string> params;
        params["threshold1"] = std::to_string(threshold1);
        params["threshold2"] = std::to_string(threshold2);
        params["apertureSize"] = std::to_string(apertureSize);
        return params;
    }

    bool setParameter(const std::string& paramName, const std::string& value) override {
        try {
            if (paramName == "threshold1") {
                threshold1 = std::stod(value);
                return true;
            } else if (paramName == "threshold2") {
                threshold2 = std::stod(value);
                return true;
            } else if (paramName == "apertureSize") {
                int val = std::stoi(value);
                if (val == 3 || val == 5 || val == 7) {
                    apertureSize = val;
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
        info["threshold1"] = "double - First threshold for hysteresis";
        info["threshold2"] = "double - Second threshold for hysteresis";
        info["apertureSize"] = "int (3, 5, or 7) - Sobel aperture size";
        return info;
    }

    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<CannyEdgeTreatment>(threshold1, threshold2, apertureSize);
    }

    bool validateInput(const cv::Mat& input) const override {
        return !input.empty() && (input.channels() == 1 || input.channels() == 3);
    }
};

#endif // CANNY_EDGE_TREATMENT_H








