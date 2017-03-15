#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <iostream>
#include <cmath>

#include "matrix.h"
#include "classifier.h"
#include "EasyBMP.h"
#include "linear.h"
#include "argvparser.h"

#define PI 3.14159
#define EPS 0.000001

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::make_pair;
using std::cout;
using std::cerr;
using std::endl;

using CommandLineProcessing::ArgvParser;

typedef vector<pair<BMP*, int> > TDataSet;
typedef vector<pair<string, int> > TFileList;
typedef vector<pair<vector<float>, int> > TFeatures;

// Load list of files and its labels from 'data_file' and
// stores it in 'file_list'
void LoadFileList(const string& data_file, TFileList* file_list) {
    ifstream stream(data_file.c_str());

    string filename;
    int label;
    
    int char_idx = data_file.size() - 1;
    for (; char_idx >= 0; --char_idx)
        if (data_file[char_idx] == '/' || data_file[char_idx] == '\\')
            break;
    string data_path = data_file.substr(0,char_idx+1);
    
    while(!stream.eof() && !stream.fail()) {
        stream >> filename >> label;
        if (filename.size())
            file_list->push_back(make_pair(data_path + filename, label));
    }

    stream.close();
}

// Load images by list of files 'file_list' and store them in 'data_set'
void LoadImages(const TFileList& file_list, TDataSet* data_set) {
    for (size_t img_idx = 0; img_idx < file_list.size(); ++img_idx) {
            // Create image
        BMP* image = new BMP();
            // Read image from file
        image->ReadFromFile(file_list[img_idx].first.c_str());
            // Add image and it's label to dataset
        data_set->push_back(make_pair(image, file_list[img_idx].second));
    }
}

// Save result of prediction to file
void SavePredictions(const TFileList& file_list,
                     const TLabels& labels, 
                     const string& prediction_file) {
        // Check that list of files and list of labels has equal size 
    assert(file_list.size() == labels.size());
        // Open 'prediction_file' for writing
    ofstream stream(prediction_file.c_str());

        // Write file names and labels to stream
    for (size_t image_idx = 0; image_idx < file_list.size(); ++image_idx)
        stream << file_list[image_idx].first << " " << labels[image_idx] << endl;
    stream.close();
}

// Exatract features from dataset.
// You should implement this function by yourself =)
void ExtractFeatures(const TDataSet& data_set, TFeatures* features) {
    for (size_t image_idx = 0; image_idx < data_set.size(); ++image_idx) {
        BMP *tmpBMP = std::get<0>(data_set[image_idx]);
        Matrix<float> gray_scale(tmpBMP->TellHeight(), tmpBMP->TellWidth());
        Matrix<float> GX(tmpBMP->TellHeight(), tmpBMP->TellWidth()), GY(tmpBMP->TellHeight(), tmpBMP->TellWidth());
        Matrix<float> G(tmpBMP->TellHeight(), tmpBMP->TellWidth()), GN(tmpBMP->TellHeight(), tmpBMP->TellWidth());
        Matrix<float> sobel_horizontal = { -1, 0, 1};
        Matrix<float> sobel_vertical(3, 1);
        sobel_vertical(0, 0) = 1;
        sobel_vertical(1, 0) = 0;
        sobel_vertical(2, 0) = -1;

        for (int i = 0; i < tmpBMP->TellHeight(); i++)
            for (int j = 0; j < tmpBMP->TellWidth(); j++)
                gray_scale(i, j) = tmpBMP->GetPixel(j, i).Red * 0.299 + tmpBMP->GetPixel(j, i).Green * 0.587 + tmpBMP->GetPixel(j, i).Blue * 0.114;


        UnaryMatrixOperator gradient_abs(sobel_horizontal);
        UnaryMatrixOperator gradient_theta(sobel_vertical);
        GX = gray_scale.unary_map(gradient_abs);
        GY = gray_scale.unary_map(gradient_theta);
        //cout<<"ooo"<<endl;

        for (int i = 0; i < tmpBMP->TellHeight(); i++)
            for (int j = 0; j < tmpBMP->TellWidth(); j++)
            {
                G(i,j) = sqrt(GX(i,j)*GX(i,j) + GY(i,j)*GY(i,j));
                GN(i,j) = fabs(atan(GY(i,j)/GX(i,j)));
            }
        
        //база
        uint count_sector = 9;
        uint count_cols = 13;
        uint count_rows = 13;
        
        uint storonaX = (tmpBMP->TellWidth())/count_cols; 
        uint storonaY = (tmpBMP->TellHeight())/count_rows;

        std::vector<vector<float>> table;
        //cout<<table.size()<<endl;
        uint m = 0;
        //uint count = 0;
        

        for (uint k = 0; k < count_rows; k++){
            for(uint l = 0; l < count_cols; l++)
            {
                std::vector<float> gistogram(count_sector, 0);
                for(uint i = k*storonaY; i < (k+1)*storonaY; i++){
                    for (uint j = l*storonaX; j < (l+1)*storonaX; j++)
                    {
                        if((GN(i, j) - PI) < EPS) m = 8;
                        else m = (GN(i,j)*count_sector)/PI;
                        gistogram[m] += G(i, j); 
                        
                    }

                }
                table.push_back(gistogram);
            }
        }

        //нормализация
        uint count = 0;

        for (uint k = 0; k < count_rows; k++){
            for(uint l = 0; l < count_cols; l++)
            {
                for(uint i = k*storonaY; i < (k+1)*storonaY; i++)
                for (uint j = l*storonaX; j < (l+1)*storonaX; j++)
                {
                    float norma = 0;
                    for (m = 0;m < count_sector; m++)
                    {
                        norma = norma + (table[count][m])*(table[count][m]);
                    }
                    norma = sqrt(norma);
                    if(norma > EPS){
                        for (m = 0;m < count_sector; m++)
                        {
                            table[count][m] = (table[count][m])/norma;
                        }
                    }
                }
                count++;
            }
        }


        vector<float> one_image_features;
        count = 0;
        for(uint k = 0; k < count_rows*count_cols; k++){
            for (uint i = 0; i < 9; ++i){
                one_image_features.push_back(table[k][i]);
            }
        }


        /*
        //1 доп
        uint kernel_vert_radius = 1;
        uint kernel_hor_radius = 1;


        Matrix<float> extra_image = gray_scale.extra_borders(kernel_vert_radius, kernel_hor_radius);

        uint count_cols_1 = 13;
        uint count_rows_1 = 13;
        
        uint storonaX_1 = (tmpBMP->TellWidth())/count_cols_1; 
        uint storonaY_1 = (tmpBMP->TellHeight())/count_rows_1;


        std::vector<vector<float>> table1;
                
        for (uint k = 0; k < count_rows_1; k++){
            for(uint l = 0; l < count_cols_1; l++)
                {
                    std::vector<float> gistogram1(256, 0);
                    //цикл по каждому пикселю блока
                    for(uint i = k*storonaY_1; i < (k+1)*storonaY_1; i++)
                        for (uint j = l*storonaX_1; j < (l+1)*storonaX_1; j++)
                            {
                                std::vector<uint> tmpGist(8,0);
                                //цикл для каждого пискселя, сравнение
                                if ((i > 0)&&(j > 0)&&(i < (gray_scale.n_rows-1))&&(j < (gray_scale.n_cols-1)))
                                {
                                    for (uint p = i-1; p <= i+1; p++)
                                        for(uint t =j-1; t <= j+1; t++)
                                        {
                                            if ((p != i)&&(t != j))
                                            {
                                                if (gray_scale(p,t) > gray_scale(i,j))
                                                    tmpGist.push_back(0);
                                                else
                                                    tmpGist.push_back(1);
                                            }
                                        }
                                }
                                else
                                {
                                        if (extra_image(i,j) > gray_scale(i,j))
                                            tmpGist.push_back(0);
                                        else
                                            tmpGist.push_back(1);

                                }

                                //перевод в число
                                uint h = 7, chislo = 0;
                                for(uint t = 0; t < 8; t++)
                                {
                                    chislo = chislo + (tmpGist[t])*(pow(2,h));
                                    h--;
                                }

                                gistogram1[chislo]++;
                            }
                    table1.push_back(gistogram1);
                }
            }

        count = 0;
        for (uint k = 0; k < count_rows_1; k++){
            for(uint l = 0; l < count_cols_1; l++)
            {
                for(uint i = k*storonaY_1; i < (k+1)*storonaY_1; i++)
                    for (uint j = l*storonaX_1; j < (l+1)*storonaX_1; j++)
                    {
                        float norma = 0;
                        for (m = 0; m < 256; m++)
                        {
                            norma = norma + (table1[count][m])*(table1[count][m]);
                        }
                        norma = sqrt(norma);
                        if(norma > EPS){
                            for (m = 0;m < 256; m++)
                            {
                                table1[count][m] = (table1[count][m])/norma;
                            }
                        }
                    }
                    count++;
            }
        }

        for(uint k = 0; k < count_rows*count_cols; k++)
            for (uint i = 0; i < 257; ++i)
                one_image_features.push_back(table1[k][i]);
        */

        //2 доп
        uint count_cols_2 = 8;
        uint count_rows_2 = 8;
        
        uint storonaX_2 = (tmpBMP->TellWidth())/count_cols_2; 
        uint storonaY_2 = (tmpBMP->TellHeight())/count_rows_2;

        float srRed = 0, srGreen = 0, srBlue = 0, srTmp = 0;

        for (uint k = 0; k < count_rows_2; k++){
            for(uint l = 0; l < count_cols_2; l++)
            {
                //std::vector<float> gistogram2(3, 0);
                for(uint i = k*storonaY_2; i < (k+1)*storonaY_2; i++){
                    for (uint j = l*storonaX_2; j < (l+1)*storonaX_2; j++)
                    {
                        srRed = srRed + tmpBMP->GetPixel(j, i).Red;
                        srGreen = srGreen + tmpBMP->GetPixel(j, i).Green;
                        srBlue = srBlue + tmpBMP->GetPixel(j, i).Blue;
                    }
                }
                srTmp = srRed/(storonaX_2*storonaY_2*255);
                one_image_features.push_back(srTmp);
                srTmp = srGreen/(storonaX_2*storonaY_2*255);
                one_image_features.push_back(srTmp);
                srTmp = srBlue/(storonaX_2*storonaY_2*255);
                one_image_features.push_back(srTmp);
                srRed = 0; srGreen = 0; srBlue = 0;
            }
        }

        //заброс
        features->push_back(make_pair(one_image_features, std::get<1>(data_set[image_idx])));
    }
}

// Clear dataset structure
void ClearDataset(TDataSet* data_set) {
        // Delete all images from dataset
    for (size_t image_idx = 0; image_idx < data_set->size(); ++image_idx)
        delete (*data_set)[image_idx].first;
        // Clear dataset
    data_set->clear();
}

// Train SVM classifier using data from 'data_file' and save trained model
// to 'model_file'
void TrainClassifier(const string& data_file, const string& model_file) {
        // List of image file names and its labels
    TFileList file_list;
        // Structure of images and its labels
    TDataSet data_set;
        // Structure of features of images and its labels
    TFeatures features;
        // Model which would be trained
    TModel model;
        // Parameters of classifier
    TClassifierParams params;
    
        // Load list of image file names and its labels
    LoadFileList(data_file, &file_list);
        // Load images
    LoadImages(file_list, &data_set);
        // Extract features from images
    ExtractFeatures(data_set, &features);

        // PLACE YOUR CODE HERE
        // You can change parameters of classifier here
    params.C = 0.1;
    TClassifier classifier(params);
        // Train classifier
    classifier.Train(features, &model);
        // Save model to file
    model.Save(model_file);
        // Clear dataset structure
    ClearDataset(&data_set);
}

// Predict data from 'data_file' using model from 'model_file' and
// save predictions to 'prediction_file'
void PredictData(const string& data_file,
                 const string& model_file,
                 const string& prediction_file) {
        // List of image file names and its labels
    TFileList file_list;
        // Structure of images and its labels
    TDataSet data_set;
        // Structure of features of images and its labels
    TFeatures features;
        // List of image labels
    TLabels labels;

        // Load list of image file names and its labels
    LoadFileList(data_file, &file_list);
        // Load images
    LoadImages(file_list, &data_set);
        // Extract features from images
    ExtractFeatures(data_set, &features);

        // Classifier 
    TClassifier classifier = TClassifier(TClassifierParams());
        // Trained model
    TModel model;
        // Load model from file
    model.Load(model_file);
        // Predict images by its features using 'model' and store predictions
        // to 'labels'
    classifier.Predict(features, model, &labels);

        // Save predictions
    SavePredictions(file_list, labels, prediction_file);
        // Clear dataset structure
    ClearDataset(&data_set);
}

int main(int argc, char** argv) {
    // Command line options parser
    ArgvParser cmd;
        // Description of program
    cmd.setIntroductoryDescription("Machine graphics course, task 2. CMC MSU, 2014.");
        // Add help option
    cmd.setHelpOption("h", "help", "Print this help message");
        // Add other options
    cmd.defineOption("data_set", "File with dataset",
        ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    cmd.defineOption("model", "Path to file to save or load model",
        ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    cmd.defineOption("predicted_labels", "Path to file to save prediction results",
        ArgvParser::OptionRequiresValue);
    cmd.defineOption("train", "Train classifier");
    cmd.defineOption("predict", "Predict dataset");
        
        // Add options aliases
    cmd.defineOptionAlternative("data_set", "d");
    cmd.defineOptionAlternative("model", "m");
    cmd.defineOptionAlternative("predicted_labels", "l");
    cmd.defineOptionAlternative("train", "t");
    cmd.defineOptionAlternative("predict", "p");

        // Parse options
    int result = cmd.parse(argc, argv);

        // Check for errors or help option
    if (result) {
        cout << cmd.parseErrorDescription(result) << endl;
        return result;
    }

        // Get values 
    string data_file = cmd.optionValue("data_set");
    string model_file = cmd.optionValue("model");
    bool train = cmd.foundOption("train");
    bool predict = cmd.foundOption("predict");

        // If we need to train classifier
    if (train)
        TrainClassifier(data_file, model_file);
        // If we need to predict data
    if (predict) {
            // You must declare file to save images
        if (!cmd.foundOption("predicted_labels")) {
            cerr << "Error! Option --predicted_labels not found!" << endl;
            return 1;
        }
            // File to save predictions
        string prediction_file = cmd.optionValue("predicted_labels");
            // Predict data
        PredictData(data_file, model_file, prediction_file);
    }
}