#ifndef SHARPEN_TREATMENT_H
#define SHARPEN_TREATMENT_H

#include "../Treatment.h"

/**
 * @brief Sharpens an image using a kernel convolution
 * 
 * Enhances edges and fine details in the image using a sharpening kernel.
 * The strength parameter controls how aggressive the sharpening is.
 */
class SharpenTreatment : public Treatment {
private:
    double strength;  // Sharpening strength (0.0 to 1.0+)

public:
    SharpenTreatment(double s = 1.0) : strength(s) {}

    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        
        // Create sharpening kernel
        // Basic sharpening kernel:
        //  0  -1   0
        // -1   5  -1
        //  0  -1   0
        // We scale it based on strength
        cv::Mat kernel = (cv::Mat_<float>(3, 3) << 
            0, -1 * strength, 0,
            -1 * strength, 1 + 4 * strength, -1 * strength,
            0, -1 * strength, 0
        );
        
        cv::filter2D(input, output, -1, kernel);
        return output;
    }

    std::string getName() const override {
        return "Sharpen";
    }

    std::string getDescription() const override {
        return "Enhances edges and fine details in the image";
    }

    std::map<std::string, std::string> getParameters() const override {
        std::map<std::string, std::string> params;
        params["strength"] = std::to_string(strength);
        return params;
    }

    bool setParameter(const std::string& paramName, const std::string& value) override {
        try {
            if (paramName == "strength") {
                double val = std::stod(value);
                if (val >= 0.0) {
                    strength = val;
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
        info["strength"] = "double - Sharpening strength (0.0 to 1.0+, typical 0.5-2.0)";
        return info;
    }

    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<SharpenTreatment>(strength);
    }
};

#endif // SHARPEN_TREATMENT_H








