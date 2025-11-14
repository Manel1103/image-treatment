#ifndef TREATMENT_H
#define TREATMENT_H

#include <opencv2/opencv.hpp>
#include <string>
#include <map>
#include <memory>
#include <vector>

/**
 * @brief Base class for all image treatments
 * 
 * This abstract class defines the interface for all image processing operations.
 * Each treatment can have configurable parameters and must implement the process() method.
 */
class Treatment {
public:
    virtual ~Treatment() = default;

    /**
     * @brief Process an input image and return the result
     * @param input The input image (cv::Mat)
     * @return The processed image
     */
    virtual cv::Mat process(const cv::Mat& input) = 0;

    /**
     * @brief Get the name of this treatment
     * @return Treatment name as string
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Get a description of this treatment
     * @return Description string
     */
    virtual std::string getDescription() const = 0;

    /**
     * @brief Get all parameters for this treatment
     * @return Map of parameter names to their current values (as strings)
     */
    virtual std::map<std::string, std::string> getParameters() const = 0;

    /**
     * @brief Set a parameter value
     * @param paramName The parameter name
     * @param value The new value (as string, will be converted internally)
     * @return true if parameter was set successfully, false otherwise
     */
    virtual bool setParameter(const std::string& paramName, const std::string& value) = 0;

    /**
     * @brief Get the list of available parameter names with their types
     * @return Map of parameter names to their types/descriptions
     */
    virtual std::map<std::string, std::string> getParameterInfo() const = 0;

    /**
     * @brief Clone this treatment (for copying the processing chain)
     * @return A unique_ptr to a new instance of this treatment
     */
    virtual std::unique_ptr<Treatment> clone() const = 0;

    /**
     * @brief Check if the treatment can process the given input
     * @param input The input image
     * @return true if input is valid, false otherwise
     */
    virtual bool validateInput(const cv::Mat& input) const {
        return !input.empty();
    }
};

#endif // TREATMENT_H

