#include <iostream>
#include <opencv2/opencv.hpp>
#include <memory>
#include <vector>
#include <chrono>
#include <sstream>
#include <ctime>
#include "ImageSource.h"
#include "Treatment.h"
#include "TreatmentChain.h"

// Include all treatment implementations
#include "treatments/GaussianBlurTreatment.h"
#include "treatments/CannyEdgeTreatment.h"
#include "treatments/ThresholdTreatment.h"
#include "treatments/BrightnessTreatment.h"
#include "treatments/MedianBlurTreatment.h"
#include "treatments/GrayscaleTreatment.h"
#include "treatments/SharpenTreatment.h"
#include "treatments/ErosionTreatment.h"
#include "treatments/DilationTreatment.h"
#include "treatments/MosaicTreatment.h"

// Helper function to resize image to fit screen
cv::Mat resizeForDisplay(const cv::Mat& img, int maxWidth = 1280, int maxHeight = 720) {
    if (img.empty()) return img;
    
    int width = img.cols;
    int height = img.rows;
    
    // If image is already smaller than max dimensions, return as is
    if (width <= maxWidth && height <= maxHeight) {
        return img;
    }
    
    // Calculate scaling factor to fit within max dimensions while maintaining aspect ratio
    double scale = std::min(static_cast<double>(maxWidth) / width, 
                           static_cast<double>(maxHeight) / height);
    
    int newWidth = static_cast<int>(width * scale);
    int newHeight = static_cast<int>(height * scale);
    
    cv::Mat resized;
    cv::resize(img, resized, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_AREA);
    return resized;
}

void testWebcam();
void testTreatmentChain();
void testImageFromFile();
void testTreatmentFromFile();

int main() {
    std::cout << "\n==============================================================\n";
    std::cout << "        SYSTEME DE TRAITEMENT D'IMAGES - MENU DE TEST\n";
    std::cout << "==============================================================\n";
    
    int choice;
    
    while (true) {
        std::cout << "\n==========================================\n";
        std::cout << "     MENU DE TEST - SYSTEME DE TRAITEMENT\n";
        std::cout << "==========================================\n";
        std::cout << "\n1. Tester la webcam (capture simple)\n";
        std::cout << "2. Tester la webcam avec traitement\n";
        std::cout << "3. Charger une image depuis un fichier\n";
        std::cout << "4. Traiter une image depuis un fichier\n";
        std::cout << "0. Quitter\n";
        std::cout << "\nVotre choix: ";
        
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                testWebcam();
                break;
            case 2:
                testTreatmentChain();
                break;
            case 3:
                testImageFromFile();
                break;
            case 4:
                testTreatmentFromFile();
                break;
            case 0:
                std::cout << "\nAu revoir!\n";
                return 0;
            default:
                std::cout << "[ERREUR] Choix invalide!\n";
        }
        
        std::cout << "\nAppuyez sur Entrée pour continuer...";
        std::cin.ignore();
        std::cin.get();
    }
    
    return 0;
}

void testWebcam() {
    std::cout << "\n==========================================\n";
    std::cout << "           TEST: CAPTURE WEBCAM SIMPLE\n";
    std::cout << "==========================================\n";
    
    std::cout << "\nQuel device ID utiliser pour la webcam? (généralement 0 ou 1): ";
    int deviceId;
    std::cin >> deviceId;
    
    WebcamImageSource webcam(deviceId);
    
    if (!webcam.isAvailable()) {
        std::cerr << "[ERREUR] Webcam non disponible!\n";
        std::cerr << "Verifications:\n";
        std::cerr << "  1. La webcam est connectee\n";
        std::cerr << "  2. Aucun autre programme n'utilise la webcam\n";
        std::cerr << "  3. Les permissions sont accordees\n";
        return;
    }
    
    std::cout << "[OK] Webcam disponible: " << webcam.getDescription() << std::endl;
    std::cout << "Capture d'une image..." << std::endl;
    
    cv::Mat frame = webcam.getStableImage();
    
    if (frame.empty()) {
        std::cerr << "[ERREUR] Impossible de capturer une image!" << std::endl;
        return;
    }
    
    std::cout << "[OK] Image capturee: " << frame.cols << "x" << frame.rows << std::endl;
    
    cv::imshow("Webcam - Image Capturée", resizeForDisplay(frame));
    std::cout << "Image affichée. Appuyez sur une touche pour fermer...\n";
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    // Sauvegarder
    cv::imwrite("webcam_capture.jpg", frame);
    std::cout << "[OK] Image sauvegardee: webcam_capture.jpg\n";
    std::cout << "[OK] Test termine!\n";
}

void testTreatmentChain() {
    std::cout << "\n==========================================\n";
    std::cout << "   TEST: CHAINE DE TRAITEMENTS AVEC WEBCAM\n";
    std::cout << "==========================================\n";
    
    std::cout << "\nQuel device ID utiliser pour la webcam? (généralement 0 ou 1): ";
    int deviceId;
    std::cin >> deviceId;
    
    WebcamImageSource webcam(deviceId);
    
    if (!webcam.isAvailable()) {
        std::cerr << "[ERREUR] Webcam non disponible!\n";
        return;
    }
    
    std::cout << "[OK] Webcam disponible: " << webcam.getDescription() << std::endl;
    std::cout << "Capture d'une image..." << std::endl;
    
    cv::Mat frame = webcam.getStableImage();
    
    if (frame.empty()) {
        std::cerr << "[ERREUR] Impossible de capturer une image!" << std::endl;
        return;
    }
    
    std::cout << "[OK] Image capturee: " << frame.cols << "x" << frame.rows << std::endl;
    
    // Créer une chaîne de traitements prédéfinie
    TreatmentChain chain;
    
    std::cout << "\n--- Construction de la chaîne de traitements ---\n";
    std::cout << "1. Grayscale\n";
    std::cout << "2. Gaussian Blur\n";
    std::cout << "3. Canny Edge Detection\n";
    
    chain.addTreatment(std::make_unique<GrayscaleTreatment>());
    chain.addTreatment(std::make_unique<GaussianBlurTreatment>(5, 1.0, 1.0));
    chain.addTreatment(std::make_unique<CannyEdgeTreatment>(50, 150, 3));
    
    std::cout << "[OK] Chaine creee avec " << chain.getTreatmentCount() << " traitements\n";
    
    // Traiter l'image
    std::cout << "\nTraitement en cours...\n";
    cv::Mat result = chain.processChain(frame);
    
    // Afficher les résultats intermédiaires
    std::cout << "\n--- Affichage des résultats intermédiaires ---\n";
    for (size_t i = 0; i <= chain.getTreatmentCount(); i++) {
        cv::Mat intermediate = chain.getIntermediateResult(i);
        std::string windowName = "Étape " + std::to_string(i);
        if (i == 0) {
            windowName += " - Original";
        } else {
            windowName += " - " + chain.getTreatment(i - 1)->getName();
        }
        cv::imshow(windowName, resizeForDisplay(intermediate));
        std::cout << "  " << windowName << " (" << intermediate.cols << "x" << intermediate.rows << ")\n";
    }
    
    std::cout << "\nAppuyez sur une touche pour fermer...\n";
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    // Créer le dossier "image" s'il n'existe pas
    std::string outputFolder = "image";
    
    #ifdef _WIN32
        std::string createDirCmd = "if not exist \"" + outputFolder + "\" mkdir \"" + outputFolder + "\"";
        system(createDirCmd.c_str());
    #else
        std::string createDirCmd = "mkdir -p " + outputFolder;
        system(createDirCmd.c_str());
    #endif
    
    // Demander si on veut sauvegarder
    std::cout << "\nVoulez-vous sauvegarder le résultat? (o/n): ";
    char save;
    std::cin >> save;
    if (save == 'o' || save == 'O') {
        // Générer un nom de fichier avec timestamp
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << outputFolder << "/webcam_result_" << timestamp << ".jpg";
        std::string filename = ss.str();
        
        if (cv::imwrite(filename, result)) {
            std::cout << "[OK] Resultat sauvegarde dans: " << filename << "\n";
            std::cout << "Dossier de sortie: " << outputFolder << "/\n";
        } else {
            std::cout << "[ERREUR] Erreur lors de la sauvegarde!\n";
        }
    }
    
    std::cout << "[OK] Test termine!\n";
}

void testImageFromFile() {
    std::cout << "\n==========================================\n";
    std::cout << "        TEST: CHARGER IMAGE DEPUIS FICHIER\n";
    std::cout << "==========================================\n";
    
    std::cout << "\nEntrez le chemin du fichier image: ";
    std::string filepath;
    std::cin.ignore();
    std::getline(std::cin, filepath);
    
    FileImageSource source(filepath);
    
    if (!source.isAvailable()) {
        std::cerr << "[ERREUR] Impossible de charger l'image!\n";
        std::cerr << "Verifiez:\n";
        std::cerr << "  1. Le chemin est correct\n";
        std::cerr << "  2. Le fichier existe\n";
        std::cerr << "  3. Le format est supporte (.jpg, .png, .bmp)\n";
        return;
    }
    
    cv::Mat image = source.getImage();
    
    if (image.empty()) {
        std::cerr << "[ERREUR] Image vide!\n";
        return;
    }
    
    std::cout << "[OK] Image chargee: " << image.cols << "x" << image.rows << std::endl;
    std::cout << "  Source: " << source.getDescription() << std::endl;
    std::cout << "  Type: " << (image.channels() == 1 ? "Grayscale" : "Color") << std::endl;
    
    cv::imshow("Image Chargée", resizeForDisplay(image));
    std::cout << "\nAppuyez sur une touche pour fermer...\n";
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    std::cout << "[OK] Test termine!\n";
}

void testTreatmentFromFile() {
    std::cout << "\n==========================================\n";
    std::cout << "   TEST: TRAITEMENT IMAGE DEPUIS FICHIER\n";
    std::cout << "==========================================\n";
    
    std::cout << "\nEntrez le chemin du fichier image: ";
    std::string filepath;
    std::cin.ignore();
    std::getline(std::cin, filepath);
    
    FileImageSource source(filepath);
    
    if (!source.isAvailable()) {
        std::cerr << "[ERREUR] Impossible de charger l'image!\n";
        return;
    }
    
    cv::Mat image = source.getImage();
    
    if (image.empty()) {
        std::cerr << "[ERREUR] Image vide!\n";
        return;
    }
    
    std::cout << "[OK] Image chargee: " << image.cols << "x" << image.rows << std::endl;
    
    // Afficher l'image originale
    cv::imshow("Original", resizeForDisplay(image));
    std::cout << "Image originale affichée. Appuyez sur une touche pour continuer...\n";
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    // Menu de sélection de traitements
    TreatmentChain chain;
    bool addingTreatments = true;
    
    std::cout << "\n==========================================\n";
    std::cout << "      CONSTRUCTION DE LA CHAINE DE TRAITEMENTS\n";
    std::cout << "==========================================\n";
    std::cout << "\nAjoutez autant de traitements que vous voulez!\n";
    
    while (addingTreatments) {
        std::cout << "\n--- Menu des traitements ---\n";
        std::cout << "1. Grayscale (Niveaux de gris)\n";
        std::cout << "2. Gaussian Blur (Flou gaussien)\n";
        std::cout << "3. Median Blur (Flou médian)\n";
        std::cout << "4. Canny Edge Detection (Détection de contours)\n";
        std::cout << "5. Threshold (Seuillage)\n";
        std::cout << "6. Brightness/Contrast (Luminosité/Contraste)\n";
        std::cout << "7. Sharpen (Accentuation)\n";
        std::cout << "8. Erosion (Érosion morphologique)\n";
        std::cout << "9. Dilation (Dilatation morphologique)\n";
        std::cout << "10. Mosaic Effect (Effet mosaïque/pixellisation)\n";
        std::cout << "0. Terminer et traiter l'image\n";
        
        if (chain.getTreatmentCount() > 0) {
            std::cout << "\nChaine actuelle (" << chain.getTreatmentCount() << " traitement(s)):\n";
            auto names = chain.getTreatmentNames();
            for (size_t i = 0; i < names.size(); i++) {
                std::cout << "   " << (i + 1) << ". " << names[i] << "\n";
            }
        }
        
        std::cout << "\nVotre choix: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 0) {
            addingTreatments = false;
        } else {
            switch (choice) {
                case 1:
                    chain.addTreatment(std::make_unique<GrayscaleTreatment>());
                    std::cout << "[OK] Grayscale ajoute\n";
                    break;
                case 2:
                    chain.addTreatment(std::make_unique<GaussianBlurTreatment>(5, 1.0, 1.0));
                    std::cout << "[OK] Gaussian Blur ajoute\n";
                    break;
                case 3:
                    chain.addTreatment(std::make_unique<MedianBlurTreatment>(5));
                    std::cout << "[OK] Median Blur ajoute\n";
                    break;
                case 4:
                    chain.addTreatment(std::make_unique<CannyEdgeTreatment>(50, 150, 3));
                    std::cout << "[OK] Canny Edge Detection ajoute\n";
                    break;
                case 5:
                    chain.addTreatment(std::make_unique<ThresholdTreatment>(127, 255, cv::THRESH_BINARY));
                    std::cout << "[OK] Threshold ajoute\n";
                    break;
                case 6:
                    chain.addTreatment(std::make_unique<BrightnessTreatment>(1.0, 0));
                    std::cout << "[OK] Brightness/Contrast ajoute\n";
                    break;
                case 7:
                    chain.addTreatment(std::make_unique<SharpenTreatment>(1.0));
                    std::cout << "[OK] Sharpen ajoute\n";
                    break;
                case 8:
                    chain.addTreatment(std::make_unique<ErosionTreatment>(3, cv::MORPH_RECT, 1));
                    std::cout << "[OK] Erosion ajoute\n";
                    break;
                case 9:
                    chain.addTreatment(std::make_unique<DilationTreatment>(3, cv::MORPH_RECT, 1));
                    std::cout << "[OK] Dilation ajoute\n";
                    break;
                case 10:
                    chain.addTreatment(std::make_unique<MosaicTreatment>(10));
                    std::cout << "[OK] Mosaic Effect ajoute\n";
                    break;
                default:
                    std::cout << "[ERREUR] Choix invalide!\n";
            }
            
            if (choice >= 1 && choice <= 10) {
                std::cout << "Total de traitements dans la chaîne: " << chain.getTreatmentCount() << "\n";
            }
        }
    }
    
    if (chain.getTreatmentCount() == 0) {
        std::cout << "[ERREUR] Aucun traitement ajoute. Fin du test.\n";
        return;
    }
    
    // Traiter l'image
    std::cout << "\n==========================================\n";
    std::cout << "           TRAITEMENT DE L'IMAGE\n";
    std::cout << "==========================================\n";
    
    std::cout << "\nChaîne finale:\n";
    auto finalNames = chain.getTreatmentNames();
    for (size_t i = 0; i < finalNames.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << finalNames[i] << "\n";
    }
    
    std::cout << "\nApplication de " << chain.getTreatmentCount() << " traitement(s)...\n";
    
    cv::Mat result;
    try {
        result = chain.processChain(image);
        
        if (result.empty()) {
            std::cout << "[ERREUR] Le resultat est vide!\n";
            return;
        }
        
        std::cout << "[OK] Traitement termine!\n";
        std::cout << "  Image resultante: " << result.cols << "x" << result.rows << "\n";
    } catch (const std::exception& e) {
        std::cout << "[ERREUR] Erreur lors du traitement: " << e.what() << "\n";
        return;
    }
    
    // Afficher tous les résultats intermédiaires
    std::cout << "\n==========================================\n";
    std::cout << "      RESULTATS INTERMEDIAIRES\n";
    std::cout << "==========================================\n";
    
    std::cout << "\nAffichage de toutes les étapes:\n";
    
    for (size_t i = 0; i <= chain.getTreatmentCount(); i++) {
        cv::Mat intermediate = chain.getIntermediateResult(i);
        
        if (intermediate.empty()) {
            std::cout << "  [WARNING] Etape " << i << " est vide!\n";
            continue;
        }
        
        std::string windowName = "Etape " + std::to_string(i);
        std::string description;
        
        if (i == 0) {
            windowName += " - Original";
            description = "Original";
        } else {
            std::string treatmentName = chain.getTreatment(i - 1)->getName();
            windowName += " - " + treatmentName;
            description = treatmentName;
        }
        
        cv::imshow(windowName, resizeForDisplay(intermediate));
        
        std::cout << "  [OK] Etape " << i << ": " << description;
        std::cout << " (" << intermediate.cols << "x" << intermediate.rows;
        std::cout << ", " << intermediate.channels() << " canal(aux))\n";
    }
    
    std::cout << "\n[INFO] " << (chain.getTreatmentCount() + 1) << " fenetre(s) affichee(s)";
    std::cout << " (Original + " << chain.getTreatmentCount() << " traitement(s))\n";
    
    std::cout << "\nAppuyez sur une touche pour fermer les fenêtres...\n";
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    // Créer le dossier "image" s'il n'existe pas
    std::string outputFolder = "image";
    
    #ifdef _WIN32
        std::string createDirCmd = "if not exist \"" + outputFolder + "\" mkdir \"" + outputFolder + "\"";
        system(createDirCmd.c_str());
    #else
        std::string createDirCmd = "mkdir -p " + outputFolder;
        system(createDirCmd.c_str());
    #endif
    
    // Demander si on veut sauvegarder
    std::cout << "\nVoulez-vous sauvegarder le résultat final? (o/n): ";
    char save;
    std::cin >> save;
    
    if (save == 'o' || save == 'O') {
        // Générer un nom de fichier avec timestamp
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << outputFolder << "/result_" << timestamp << ".jpg";
        std::string outputFile = ss.str();
        
        if (cv::imwrite(outputFile, result)) {
            std::cout << "[OK] Resultat sauvegarde dans: " << outputFile << "\n";
            std::cout << "Dossier de sortie: " << outputFolder << "/\n";
        } else {
            std::cout << "[ERREUR] Erreur lors de la sauvegarde!\n";
        }
    }
    
    std::cout << "\n[OK] Test termine!\n";
}
