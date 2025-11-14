#ifndef BRIGHTNESS_TREATMENT_H
#define BRIGHTNESS_TREATMENT_H

#include "../Treatment.h"
#include <sstream>

/**
 * @brief Adjusts the brightness and contrast of an image
 * 
 * Formula: output = alpha * input + beta
 * - alpha: contrast control (1.0-3.0 typical)
 * - beta: brightness control (-100 to 100 typical)
 */
class BrightnessTreatment : public Treatment {
private:
    double alpha;  // Contrast multiplier
    double beta;   // Brightness offset

public:
    BrightnessTreatment(double a = 1.0, double b = 0.0)
        : alpha(a), beta(b) {}

    cv::Mat process(const cv::Mat& input) override {
        cv::Mat output;
        input.convertTo(output, -1, alpha, beta);
        return output;
    }

    std::string getName() const override {
        return "Brightness/Contrast";
    }

    std::string getDescription() const override {
        return "Adjusts brightness and contrast (output = alpha * input + beta)";
    }

    std::map<std::string, std::string> getParameters() const override {
        std::map<std::string, std::string> params;
        params["alpha"] = std::to_string(alpha);
        params["beta"] = std::to_string(beta);
        return params;
    }

    bool setParameter(const std::string& paramName, const std::string& value) override {
        try {
            if (paramName == "alpha") {
                alpha = std::stod(value);
                return true;
            } else if (paramName == "beta") {
                beta = std::stod(value);
                return true;
            }
        } catch (...) {
            return false;
        }
        return false;
    }

    std::map<std::string, std::string> getParameterInfo() const override {
        std::map<std::string, std::string> info;
        info["alpha"] = "double - Contrast control (1.0-3.0 typical)";
        info["beta"] = "double - Brightness control (-100 to 100 typical)";
        return info;
    }

    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<BrightnessTreatment>(alpha, beta);
    }
};

#endif // BRIGHTNESS_TREATMENT_H








