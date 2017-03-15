#include "align.h"
#include <string>

using std::string;
using std::cout;
using std::endl;

long int maxsimum(Image blue, Image green, int k, int l)
{
    long int summ = 0;
    uint I1,I2;

    if ((k >= 0) && (l >= 0))
    {
        for(uint i = k; i < blue.n_cols; i++)
        for (uint j = 0; j < blue.n_rows-l; j++)
        {
            I1 = std::get<1>(blue(j,i));
            I2 = std::get<1>(green(j+l,i-k));
            summ = summ + (I1*I2);
        
        }
    }

    if ((k >= 0) && (l < 0))
    {
        for(uint i = k; i < blue.n_cols; i++)
        for (uint j = abs(l); j < blue.n_rows; j++)
        {
            I1 = std::get<1>(blue(j,i));
            I2 = std::get<1>(green(j+l,i-k));
            summ = summ + (I1*I2);
        
        }
    }

    if ((k < 0) && (l < 0))
    {
        for(uint i = 0; i < blue.n_cols - abs(k); i++)
        for (uint j = abs(l); j < blue.n_rows; j++)
        {
            I1 = std::get<1>(blue(j,i));
            I2 = std::get<1>(green(j+l,i+abs(k)));
            summ = summ + (I1*I2);
        
        }
    }
    
    if ((k < 0) && (l >= 0))
    {
        for(uint i = 0; i < blue.n_cols - abs(k); i++)
        for (uint j = 0; j < blue.n_rows - l; j++)
        {
            I1 = std::get<1>(blue(j,i));
            I2 = std::get<1>(green(j+l,i+abs(k)));
            summ = summ + (I1*I2);
        
        }
    }

    return summ;
}

uint minimum(Image blue, Image green, int k, int l)
{
    uint summ = 0;
    uint I1,I2;
    
    uint p = (blue.n_cols-k)*(blue.n_rows-l);

    if ((k >= 0) && (l >= 0))
    {
        for(uint i = k; i < blue.n_cols; i++)
        for (uint j = 0; j < blue.n_rows-l; j++)
        {
            I1 = std::get<1>(blue(j,i));
            I2 = std::get<1>(green(j+l,i-k));
            summ = summ + ((I1-I2)*(I1-I2));
        
        }
    }

    if ((k >= 0) && (l < 0))
    {
        for(uint i = k; i < blue.n_cols; i++)
        for (uint j = abs(l); j < blue.n_rows; j++)
        {
            I1 = std::get<1>(blue(j,i));
            I2 = std::get<1>(green(j+l,i-k));
            summ = summ + ((I1-I2)*(I1-I2));
        
        }
    }

    if ((k < 0) && (l < 0))
    {
        for(uint i = 0; i < blue.n_cols - abs(k); i++)
        for (uint j = abs(l); j < blue.n_rows; j++)
        {
            I1 = std::get<1>(blue(j,i));
            I2 = std::get<1>(green(j+l,i+abs(k)));
            summ = summ + ((I1-I2)*(I1-I2));
        
        }
    }
    
    if ((k < 0) && (l >= 0))
    {
        for(uint i = 0; i < blue.n_cols - abs(k); i++)
        for (uint j = 0; j < blue.n_rows - l; j++)
        {
            I1 = std::get<1>(blue(j,i));
            I2 = std::get<1>(green(j+l,i+abs(k)));
            summ = summ + ((I1-I2)*(I1-I2));
        
        }
    }

    summ = summ/p;

    return summ;
}

Image align(Image srcImage, bool isPostprocessing, std::string postprocessingType, double fraction, bool isMirror, 
            bool isInterp, bool isSubpixel, double subScale)
{
    Image blue, green,red;

    blue = srcImage.submatrix(0,0,srcImage.n_rows/3, srcImage.n_cols);
    green = srcImage.submatrix(srcImage.n_rows/3,0, srcImage.n_rows/3, srcImage.n_cols);
    red = srcImage.submatrix((srcImage.n_rows/3)*2,0, srcImage.n_rows/3, srcImage.n_cols);

  
    uint min = 655368960;
    uint tmp;
    int min_k = 20,min_l = 20;

    cout<<"cреднеквадратичное отклониение, оптимальный сдвиг: "<<endl;
    for(int k = -15; k<16; k++)
        for(int l = -15; l < 16; l++)
        {
            tmp = minimum(red,green,k,l);
            //cout<<"k = "<<k<<" "<<"l = "<<l<<" "<<"func = "<<tmp<<endl;
            if (tmp <= min)
            {
                min = tmp;
                min_k = k;
                min_l = l;
            }
        }
    cout<<min_k<<"    "<<min_l<<endl;


    long int max = 0;
    int max_k = -20, max_l = - 20;
    long int tmpmax;
    cout<<"кросс-корреляция, оптимальный сдвиг : "<<endl;
    for(int k = -15; k<16; k++)
        for(int l = -15; l < 16; l++)
        {
            tmpmax = maxsimum(blue,green,k,l);
            //cout<<"k = "<<k<<" "<<"l = "<<l<<" "<<"func = "<<tmpmax<<endl;
            if (tmpmax >= max)
            {
                max = tmpmax;
                max_k = k;
                max_l = l;
            }
        }
    cout<<max_k<<"     "<<max_l<<endl;

    Image all(blue.n_rows, blue.n_cols); //для среднеквадратичной оценки

    //для корреляции
    //min_k = max_k;
    //min_l = max_l;

    //совмещение двух каналов red и  green по среднеквадратичной оценки
    if ((min_k >= 0)&&(min_l >= 0))// + +
    {
        for(uint i = min_k; i < blue.n_cols; i++)
            for(uint j = 0; j < blue.n_rows - min_l; j++)
            {
                std::get<0>(all(j,i)) = std::get<1>(red(j,i)); //в all кладем green канал
                std::get<1>(all(j,i)) = std::get<1>(green(j+min_l,i-min_k)); //кладем туда же green канал, соответсвующие пиксели по смещению
            }
    }

    if ((min_k >= 0)&&(min_l < 0)) // + -
    {
        for(uint i = min_k; i < blue.n_cols; i++)
            for(uint j = abs(min_l); j < blue.n_rows; j++)
            {
                std::get<0>(all(j,i)) = std::get<1>(red(j,i)); 
                std::get<1>(all(j,i)) = std::get<1>(green(j+min_l,i-min_k)); 
            }
    }

    if ((min_k < 0)&&(min_l < 0)) //- -
    {
        for(uint i = 0; i < blue.n_cols - abs(min_k); i++)
            for(uint j = abs(min_l); j < blue.n_rows; j++)
            {
                std::get<0>(all(j,i)) = std::get<1>(red(j,i)); 
                std::get<1>(all(j,i)) = std::get<1>(green(j+min_l,i+abs(min_k))); 
            }
    }

    if ((min_k < 0)&&(min_l >= 0)) // - +
    {
        for(uint i = 0; i < blue.n_cols - abs(min_k); i++)
            for(uint j = 0; j < blue.n_rows - min_l; j++)
            {
                std::get<0>(all(j,i)) = std::get<1>(red(j,i)); 
                std::get<1>(all(j,i)) = std::get<1>(green(j+min_l,i+abs(min_k))); 
            }
    }


    min = 655368960;
    min_k = 20; min_l = 20;
    cout<<"cреднеквадратичное отклониение, оптимальный сдвиг : "<<endl;
    for(int k = -15; k<16; k++)
        for(int l = -15; l < 16; l++)
        {
            tmp = minimum(all,blue,k,l);
            //cout<<"k = "<<k<<" "<<"l = "<<l<<" "<<"func = "<<tmp<<endl;
            if (tmp <= min)
            {
                min = tmp;
                min_k = k;
                min_l = l;
            }
        }
    cout<<min_k<<"    "<<min_l<<endl;

    if ((min_k >= 0)&&(min_l >= 0))// + +
    {
        for(uint i = min_k; i < blue.n_cols; i++)
            for(uint j = 0; j < blue.n_rows - min_l; j++)
                std::get<2>(all(j,i)) = std::get<1>(blue(j+min_l,i-min_k)); 
    }

    if ((min_k >= 0)&&(min_l < 0)) // + -
    {
        for(uint i = min_k; i < blue.n_cols; i++)
            for(uint j = abs(min_l); j < blue.n_rows; j++)
                std::get<2>(all(j,i)) = std::get<1>(blue(j+min_l,i-min_k));
    }

    if ((min_k < 0)&&(min_l < 0)) //- -
    {
        for(uint i = 0; i < blue.n_cols - abs(min_k); i++)
            for(uint j = abs(min_l); j < blue.n_rows; j++)
                std::get<2>(all(j,i)) = std::get<1>(blue(j+min_l,i+abs(min_k))); 
    }

    if ((min_k < 0)&&(min_l >= 0)) // - +
    {
        for(uint i = 0; i < blue.n_cols - abs(min_k); i++)
            for(uint j = 0; j < blue.n_rows - min_l; j++)
                std::get<2>(all(j,i)) = std::get<1>(blue(j+min_l,i+abs(min_k)));
    }



    /*for (uint i=0; i<srcImage.n_rows/3; i++)
        for (uint j=0; j<srcImage.n_cols; j++)
        {
            std::get<0>(red(i,j)) = std::get<1>(red(i,j));
            std::get<1>(red(i,j)) = std::get<1>(green(i,j));
            std::get<2>(red(i,j)) = std::get<2>(blue(i,j));
        }*/

    //Image gray = gray_world(all);
    //Image sharp = unsharp(all);

    return all;
}

Image sobel_x(Image src_image) {
    Matrix<double> kernel = {{-1, 0, 1},
                             {-2, 0, 2},
                             {-1, 0, 1}};
    return custom(src_image, kernel);
}

Image sobel_y(Image src_image) {
    Matrix<double> kernel = {{ 1,  2,  1},
                             { 0,  0,  0},
                             {-1, -2, -1}};
    return custom(src_image, kernel);
}

Image unsharp(Image src_image)
{
    src_image = mirror(src_image, 1);
    long int summ1 = 0; long int summ2 = 0; long int summ0 = 0;
    for(uint k = 0; k < src_image.n_cols; k++)
        for(uint l = 0; l < src_image.n_rows; l++)
        {
            summ0 = summ0 + std::get<0>(src_image(l,k))*4.333;
            summ1 = summ1 + std::get<1>(src_image(l,k))*4.333;
            summ2 = summ2 + std::get<2>(src_image(l,k))*4.333;
            if (l > 0)
            {
                summ0 = summ0 - std::get<0>(src_image(l-1,k))*0.667;
                summ1 = summ1 - std::get<1>(src_image(l-1,k))*0.667;
                summ2 = summ2 - std::get<2>(src_image(l-1,k))*0.667;
            }
            if (l < src_image.n_rows-1)
            {
                summ0 = summ0 - std::get<0>(src_image(l+1,k))*0.667;
                summ1 = summ1 - std::get<1>(src_image(l+1,k))*0.667;
                summ2 = summ2 - std::get<2>(src_image(l+1,k))*0.667;
            }
            if ((l > 0)&&(k > 0))
            {
                summ0 = summ0 - std::get<0>(src_image(l-1,k-1))*0.167;
                summ1 = summ1 - std::get<1>(src_image(l-1,k-1))*0.167;
                summ2 = summ2 - std::get<2>(src_image(l-1,k-1))*0.167;
            }
            if ((l < src_image.n_rows-1)&&(k > 0))
            {
                summ0 = summ0 - std::get<0>(src_image(l+1,k-1))*0.167;
                summ1 = summ1 - std::get<1>(src_image(l+1,k-1))*0.167;
                summ2 = summ2 - std::get<2>(src_image(l+1,k-1))*0.167;
            }
            if (k > 0)
            {
                summ0 = summ0 - std::get<0>(src_image(l,k-1))*0.667;
                summ1 = summ1 - std::get<1>(src_image(l,k-1))*0.667;
                summ2 = summ2 - std::get<2>(src_image(l,k-1))*0.667;
            }
            if ((l > 0)&&(k < src_image.n_cols-1))
            {
                summ0 = summ0 - std::get<0>(src_image(l-1,k+1))*0.167;
                summ1 = summ1 - std::get<1>(src_image(l-1,k+1))*0.167;
                summ2 = summ2 - std::get<2>(src_image(l-1,k+1))*0.167;
            }
            if (k < src_image.n_cols-1)
            {
                summ0 = summ0 - std::get<0>(src_image(l,k+1))*0.667;
                summ1 = summ1 - std::get<1>(src_image(l,k+1))*0.667;
                summ2 = summ2 - std::get<2>(src_image(l,k+1))*0.667;
            }
            if ((l < src_image.n_rows-1)&&(k < src_image.n_cols-1))
            {
                summ0 = summ0 - std::get<0>(src_image(l+1,k+1))*0.167;
                summ1 = summ1 - std::get<1>(src_image(l+1,k+1))*0.167;
                summ2 = summ2 - std::get<2>(src_image(l+1,k+1))*0.167;
            }
            //summ = summ/9;
            if (summ0 > 255) summ0 = 255;
            if (summ1 > 255) summ1 = 255;
            if (summ2 > 255) summ2 = 255;
            if (summ0 < 0) summ0 = 0;
            if (summ1 < 0) summ1 = 0;
            if (summ2 < 0) summ2 = 0;
            //cout<<"k = "<<k<<" "<<"l = "<<l<<" "<<"summ0 = "<<summ0<<" summ1 = "<<summ1<<" summ2 = "<<summ2<< endl;
            std::get<0>(src_image(l,k)) = summ0;
            std::get<1>(src_image(l,k)) = summ1;
            std::get<2>(src_image(l,k)) = summ2;
            //src_image(l,k) = std::make_tuple(summ0,summ1,summ2);
            summ0 = 0;
            summ1 = 0;
            summ2 = 0;  
        }

    return src_image;
}

Image mirror(Image src_image, uint radius)
{
    Image out(src_image.n_rows + 2*radius, src_image.n_cols + 2*radius);

    for (uint i = radius; i < src_image.n_cols + radius; i++)
        for (uint j = radius; j < src_image.n_rows + radius; j++)
            out(j, i) = src_image(j - radius, i - radius);
    
    for(uint i = 0; i < radius; i++)
        for (uint j = 0; j < src_image.n_rows; j++)
            out(j + radius, i + src_image.n_cols + radius) = src_image(j, src_image.n_cols - i - 1);

    for (uint i = 0; i < radius; i++)
        for (uint j = 0; j < src_image.n_rows; j++)
            out(j + radius, i) = src_image(j, radius - i -1);

    for (uint i = 0; i < src_image.n_cols; i++)
        for (uint j = 0; j < radius; j++)
            out(j,i+radius) = src_image(radius - j -1, i);

    for(uint i = 0; i <src_image.n_cols; i++)
        for(uint j = 0; j < radius; j++)
            out(j + src_image.n_rows + radius ,i+radius) = src_image(src_image.n_rows - j - 1,i);
    
    for(uint i = 0; i < radius; i++)
        for(uint j = 0; j < radius; j++)
            out(j,i) = src_image(radius - j, radius - i);

    for(uint i = 0; i < radius; i++)
        for(uint j = 0; j < radius; j++)
            out(j,i + src_image.n_cols + radius) = src_image(radius-j,src_image.n_cols-i-1);

    for(uint i = 0; i < radius; i++)
        for(uint j = 0; j < radius; j++)
            out(j+radius+src_image.n_rows, i) = src_image(radius - j, radius - i);

    for(uint i = 0; i < radius; i++)
        for(uint j = 0; j < radius; j++)
            out(j+radius+src_image.n_rows, i+radius+src_image.n_cols) = src_image(radius - j, src_image.n_cols - i -1);

    return out;
}

Image gray_world(Image src_image)
{
    double SR = 0, SG = 0, SB = 0, S, k1, k2, k3;
    double kolvo = src_image.n_rows*src_image.n_cols;
    for(uint i = 0; i < src_image.n_cols; i++)
        for(uint j = 0; j < src_image.n_rows; j++)
        {
            SR = SR + std::get<0>(src_image(j,i));
            SG = SG + std::get<1>(src_image(j,i));
            SB = SB + std::get<2>(src_image(j,i));
        }
    SR = SR/kolvo;
    SG = SG/kolvo;
    SB = SB/kolvo;
    S = (SR+SG+SB)/3;
    k1 = S/SR; k2 = S/SG; k3 = S/SB;
    for (uint i = 0; i<src_image.n_cols; i++)
        for(uint j = 0; j < src_image.n_rows; j++)
        {
            std::get<0>(src_image(j,i)) = std::get<0>(src_image(j,i))*k1;
            std::get<1>(src_image(j,i)) = std::get<1>(src_image(j,i))*k2;
            std::get<2>(src_image(j,i)) = std::get<2>(src_image(j,i))*k3;
        }
    return src_image;
}

Image resize(Image src_image, double scale) {
    return src_image;
}

Image custom(Image src_image, Matrix<double> kernel) {
    // Function custom is useful for making concrete linear filtrations
    // like gaussian or sobel. So, we assume that you implement custom
    // and then implement other filtrations using this function.
    // sobel_x and sobel_y are given as an example.
    return src_image;
}

Image autocontrast(Image src_image, double fraction)
{
    //автоконтраст без fraction:
    /*double tmpR,tmpG,tmpB;
    double Ymin = 999999,Ymax = 0;
    uint Y[src_image.n_cols-30][src_image.n_rows-30];

    for(uint i = 30; i < src_image.n_cols-30; i++)
        for(uint j = 30; j < src_image.n_rows-30; j++)
        {
            tmpR = std::get<0>(src_image(j,i));
            tmpG = std::get<1>(src_image(j,i));
            tmpB = std::get<2>(src_image(j,i));

            Y[i][j] = 0.2125*tmpR + 0.7154*tmpG + 0.0721*tmpB;
            if (Y[i][j] < Ymin) Ymin = Y[i][j];
            if (Y[i][j] > Ymax) Ymax = Y[i][j];
        }

    for(uint i = 30; i < src_image.n_cols-30; i++)
        for(uint j = 30; j < src_image.n_rows-30; j++)
        {
            std::get<0>(src_image(j,i)) = std::get<0>(src_image(j,i))*(1 - Ymin/Y[i][j]) * ((255-0)/(Ymax - Ymin));
            std::get<1>(src_image(j,i)) = std::get<1>(src_image(j,i))*(1 - Ymin/Y[i][j]) * ((255-0)/(Ymax - Ymin));
            std::get<2>(src_image(j,i)) = std::get<2>(src_image(j,i))*(1 - Ymin/Y[i][j]) * ((255-0)/(Ymax - Ymin));

        }
*/
      
    //с fraction

    int A[256][2];
    int Yn;
    for (uint i = 0; i < 256; i++)
    {
        A[i][1] = i;
        A[i][0] = 0;
    }

    int Prib;
    for(uint p = 0; p < src_image.n_cols; p++)
        for(uint t = 0; t < src_image.n_rows; t++)
        {
            Yn = 0.2125*std::get<0>(src_image(t,p)) + 0.7154*std::get<1>(src_image(t,p)) + 0.0721*std::get<2>(src_image(t,p));
            Prib = round(Yn);
            A[Prib][0] = A[Prib][0] + 1;
        }


    long int summ = 0;
    uint min_i = -1;
    uint max_i = -1;
    for (uint i = 0; i < 256; i++)
    {
        summ = summ + A[i][0];
        //cout<<"summ = "<<summ<<" i = "<<i<<" sr = "<<src_image.n_rows*src_image.n_cols*fraction<<endl;
        if (summ > (src_image.n_rows*src_image.n_cols*fraction))
        {
            min_i = i;
            break;
        }
    }
    summ = 0;
    for (int i = 255; i >= 0; i--)
    {
        summ = summ + A[i][0];
        if (summ > (src_image.n_rows*src_image.n_cols*fraction))
        {
            max_i = i;
            break;
        }
    }

    int tmpR,tmpG,tmpB;
    double R0,R1,R2;
    double T0,T1,T2;
    //cout<<"max = "<<max_i<<" min_i = "<<min_i<<endl;
    for(uint i = 0; i < src_image.n_cols; i++)
        for(uint j = 0; j < src_image.n_rows; j++)
        { 
            tmpR = std::get<0>(src_image(j,i));
            tmpG = std::get<1>(src_image(j,i));
            tmpB = std::get<2>(src_image(j,i));
            Yn = 0.2125*tmpR + 0.7154*tmpG + 0.0721*tmpB;  

            if (tmpR != 0)
                T0 = min_i/tmpR;
            else T0 = 0;

            if (tmpG != 0)
                T1 = min_i/tmpG;
            else T1 = 0;

            if (tmpB!= 0)
                T2 = min_i/tmpB;
            else T2 = 0;
            R0 = tmpR*(1 - T0) * ((255-0)/(max_i - min_i));
            R1 = tmpG*(1 - T1) * ((255-0)/(max_i - min_i));
            R2 = tmpB*(1 - T2) * ((255-0)/(max_i - min_i));
            if (R0 > 255) R0 = 255;
            if (R1 > 255) R1 = 255;
            if (R2 > 255) R2 = 255;
            if (R0 < 0) R0 = 0;
            if (R1 < 0) R1= 0;
            if (R2 < 0) R2 = 0;
            std::get<0>(src_image(j,i)) = R0;
            std::get<1>(src_image(j,i)) = R1;
            std::get<2>(src_image(j,i)) = R2;
            //cout<<"  R = "<<R0;
            //cout<<"  G = "<<R1;
            //cout<<"  B = "<<R2<<endl;
        }



    return src_image;
}

Image gaussian(Image src_image, double sigma, int radius)  {
    return src_image;
}

Image gaussian_separable(Image src_image, double sigma, int radius) {
    return src_image;
}

Image median(Image src_image, int radius) {
    return src_image;
}

Image median_linear(Image src_image, int radius) {
    return src_image;
}

Image median_const(Image src_image, int radius) {
    return src_image;
}

Image canny(Image src_image, int threshold1, int threshold2) {
    return src_image;
}
