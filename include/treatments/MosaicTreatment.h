#ifndef MOSAICTREATMENT_H
#define MOSAICTREATMENT_H

#include "../Treatment.h"
#include <opencv2/opencv.hpp>
#include <map>
#include <sstream>

/**
 * @class MosaicTreatment
 * @brief Applique un effet mosaïque/pixellisation à l'image
 * 
 * Ce traitement crée un effet de pixellisation en réduisant la résolution
 * de l'image puis en la réagrandissant, créant ainsi des blocs de couleur uniforme.
 */
class MosaicTreatment : public Treatment {
private:
    int blockSize;  // Taille des blocs de mosaïque (plus grand = plus pixelisé)

public:
    /**
     * @brief Constructeur
     * @param blockSize Taille des blocs de mosaïque (défaut: 10)
     *                  Valeurs recommandées: 5-50
     */
    MosaicTreatment(int blockSize = 10) : blockSize(blockSize) {
        if (blockSize < 1) {
            this->blockSize = 1;
        }
    }

    /**
     * @brief Applique l'effet mosaïque à l'image
     * @param image Image d'entrée
     * @return Image avec effet mosaïque appliqué
     */
    cv::Mat process(const cv::Mat& image) override {
        if (image.empty()) {
            return image;
        }

        cv::Mat result;
        
        // Calculer la nouvelle taille réduite
        int newWidth = std::max(1, image.cols / blockSize);
        int newHeight = std::max(1, image.rows / blockSize);
        
        // Réduire l'image
        cv::Mat smallImage;
        cv::resize(image, smallImage, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_LINEAR);
        
        // Réagrandir l'image avec interpolation nearest neighbor pour garder l'effet pixelisé
        cv::resize(smallImage, result, image.size(), 0, 0, cv::INTER_NEAREST);
        
        return result;
    }

    /**
     * @brief Retourne le nom du traitement
     * @return "Mosaic Effect"
     */
    std::string getName() const override {
        return "Mosaic Effect";
    }

    /**
     * @brief Retourne la description du traitement
     * @return Description
     */
    std::string getDescription() const override {
        return "Applies pixelation/mosaic effect to images";
    }

    /**
     * @brief Obtient tous les paramètres
     * @return Map des paramètres
     */
    std::map<std::string, std::string> getParameters() const override {
        std::map<std::string, std::string> params;
        params["blockSize"] = std::to_string(blockSize);
        return params;
    }

    /**
     * @brief Définit un paramètre (version string)
     * @param paramName Nom du paramètre
     * @param value Valeur (string)
     * @return true si réussi
     */
    bool setParameter(const std::string& paramName, const std::string& value) override {
        try {
            if (paramName == "blockSize") {
                int val = std::stoi(value);
                if (val >= 1) {
                    blockSize = val;
                    return true;
                }
            }
        } catch (...) {
            return false;
        }
        return false;
    }

    /**
     * @brief Obtient les informations sur les paramètres
     * @return Map des descriptions de paramètres
     */
    std::map<std::string, std::string> getParameterInfo() const override {
        std::map<std::string, std::string> info;
        info["blockSize"] = "int (positive) - Size of mosaic blocks (larger = more pixelated)";
        return info;
    }

    /**
     * @brief Clone le traitement
     * @return Pointeur unique vers une copie
     */
    std::unique_ptr<Treatment> clone() const override {
        return std::make_unique<MosaicTreatment>(blockSize);
    }

    /**
     * @brief Définit la taille des blocs de mosaïque
     * @param size Nouvelle taille des blocs (min: 1)
     */
    void setBlockSize(int size) {
        if (size >= 1) {
            blockSize = size;
        }
    }

    /**
     * @brief Obtient la taille actuelle des blocs
     * @return Taille des blocs
     */
    int getBlockSize() const {
        return blockSize;
    }
};

#endif // MOSAICTREATMENT_H

