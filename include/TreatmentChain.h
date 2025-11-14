#ifndef TREATMENT_CHAIN_H
#define TREATMENT_CHAIN_H

#include "Treatment.h"
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @brief Manages a chain of image treatments
 * 
 * This class maintains an ordered list of treatments and processes
 * an image through all of them sequentially.
 */
class TreatmentChain {
private:
    std::vector<std::unique_ptr<Treatment>> treatments;
    cv::Mat originalImage;
    std::vector<cv::Mat> intermediateResults;

public:
    /**
     * @brief Add a treatment to the end of the chain
     * @param treatment Unique pointer to the treatment to add
     */
    void addTreatment(std::unique_ptr<Treatment> treatment) {
        treatments.push_back(std::move(treatment));
    }

    /**
     * @brief Insert a treatment at a specific position
     * @param index Position to insert at
     * @param treatment Unique pointer to the treatment to insert
     */
    void insertTreatment(size_t index, std::unique_ptr<Treatment> treatment) {
        if (index > treatments.size()) {
            throw std::out_of_range("Index out of range");
        }
        treatments.insert(treatments.begin() + index, std::move(treatment));
    }

    /**
     * @brief Remove a treatment from the chain
     * @param index Index of the treatment to remove
     */
    void removeTreatment(size_t index) {
        if (index >= treatments.size()) {
            throw std::out_of_range("Index out of range");
        }
        treatments.erase(treatments.begin() + index);
    }

    /**
     * @brief Get the number of treatments in the chain
     * @return Number of treatments
     */
    size_t getTreatmentCount() const {
        return treatments.size();
    }

    /**
     * @brief Get a treatment at a specific index
     * @param index Index of the treatment
     * @return Pointer to the treatment
     */
    Treatment* getTreatment(size_t index) {
        if (index >= treatments.size()) {
            throw std::out_of_range("Index out of range");
        }
        return treatments[index].get();
    }

    /**
     * @brief Process an image through the entire chain
     * @param input The input image
     * @return The final processed image
     */
    cv::Mat processChain(const cv::Mat& input) {
        if (input.empty()) {
            throw std::invalid_argument("Input image is empty");
        }

        originalImage = input.clone();
        intermediateResults.clear();
        intermediateResults.push_back(originalImage);

        cv::Mat current = input;
        for (size_t i = 0; i < treatments.size(); ++i) {
            if (!treatments[i]->validateInput(current)) {
                throw std::runtime_error("Treatment " + std::to_string(i) + 
                                       " cannot process the current image");
            }
            current = treatments[i]->process(current);
            intermediateResults.push_back(current.clone());
        }

        return current;
    }

    /**
     * @brief Get the intermediate result after a specific treatment
     * @param index Index of the treatment (0 = original, 1 = after first treatment, etc.)
     * @return The intermediate image
     */
    cv::Mat getIntermediateResult(size_t index) const {
        if (index >= intermediateResults.size()) {
            throw std::out_of_range("Index out of range");
        }
        return intermediateResults[index];
    }

    /**
     * @brief Clear all treatments from the chain
     */
    void clear() {
        treatments.clear();
        intermediateResults.clear();
    }

    /**
     * @brief Get information about all treatments in the chain
     * @return Vector of treatment names
     */
    std::vector<std::string> getTreatmentNames() const {
        std::vector<std::string> names;
        for (const auto& treatment : treatments) {
            names.push_back(treatment->getName());
        }
        return names;
    }
};

#endif // TREATMENT_CHAIN_H

