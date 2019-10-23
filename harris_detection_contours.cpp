#include <opencv2/opencv.hpp>
#include <fstream>

#define B 0
#define G 1
#define R 2
#define CONTRAST 75
#define SEUIL 25
#define LAMBDA 1
using namespace cv;
using namespace std;

typedef std::tuple<int, int, int> pixel;

int vertical_mask[3][3] = {{-1, 0, 1},
                           {-1, 0, 1},
                           {-1, 0, 1}};

int horizontal_mask[3][3] = {{-1, -1, -1},
                             {0,  0,  0},
                             {1,  1,  1}};

int gaussian_mask[3][3] = {{1, 1, 1},
                           {1, 1, 1},
                           {1, 1, 1}};


void applyMatrix(Mat &frame, Mat &targetFrame, const int mask[3][3]) {

    for (int row_index = 1; row_index < (frame.rows - 1); row_index++) {
        for (int col_index = 1; col_index < (frame.cols - 1); col_index++) {
            int pixel_value = floor(
                    mask[0][0] * frame.at<uchar>(row_index - 1, col_index - 1) +
                    mask[0][1] * frame.at<uchar>(row_index - 1, col_index) +
                    mask[0][2] * frame.at<uchar>(row_index - 1, col_index + 1) +
                    mask[1][0] * frame.at<uchar>(row_index, col_index - 1) +
                    mask[1][1] * frame.at<uchar>(row_index, col_index) +
                    mask[1][2] * frame.at<uchar>(row_index, col_index + 1) +
                    mask[2][0] * frame.at<uchar>(row_index + 1, col_index - 1) +
                    mask[2][1] * frame.at<uchar>(row_index + 1, col_index) +
                    mask[2][2] * frame.at<uchar>(row_index + 1, col_index + 1) / 9);
            targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(pixel_value);
        }
    }
//    return targetFrame;
}

void gaussianBlur(Mat &frame, Mat &targetFrame, const int mask[3][3]) {

    for (int row_index = 1; row_index < (frame.rows - 1); row_index++) {
        for (int col_index = 1; col_index < (frame.cols - 1); col_index++) {

            int pixel_value = floor(
                    mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    mask[1][1] * frame.at<char>(row_index, col_index) +
                    mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    mask[2][2] * frame.at<char>(row_index + 1, col_index + 1) / 9);
            targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(pixel_value);

        }
    }
}


void
extractHarrysKeypoints(Mat &frame, Mat &targetFrame, const int horizontal_mask[3][3], const int vertical_mask[3][3]) {
    Mat horizontal_masked_image = targetFrame.clone();
    Mat vertical_masked_image = targetFrame.clone();

    for (int row_index = 1; row_index < (frame.rows - 1); row_index++) {
        for (int col_index = 1; col_index < (frame.cols - 1); col_index++) {

            int pixel_value_horizontal = abs(
                    horizontal_mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    horizontal_mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    horizontal_mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    horizontal_mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    horizontal_mask[1][1] * frame.at<char>(row_index, col_index) +
                    horizontal_mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    horizontal_mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    horizontal_mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    horizontal_mask[2][2] * frame.at<char>(row_index + 1, col_index + 1));

            int pixel_value_vertical = abs(
                    vertical_mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    vertical_mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    vertical_mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    vertical_mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    vertical_mask[1][1] * frame.at<char>(row_index, col_index) +
                    vertical_mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    vertical_mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    vertical_mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    vertical_mask[2][2] * frame.at<char>(row_index + 1, col_index + 1));

            pixel_value_horizontal >= SEUIL ? pixel_value_horizontal = pixel_value_horizontal
                                            : pixel_value_horizontal = 0;

            pixel_value_vertical >= SEUIL ? pixel_value_vertical = pixel_value_vertical
                                          : pixel_value_vertical = 0;
            if (pixel_value_horizontal > 255)
                pixel_value_horizontal = 255;
            if (pixel_value_vertical > 255)
                pixel_value_vertical = 255;

            long pixel_value_horizontal_squared = pixel_value_horizontal * pixel_value_horizontal;
            long pixel_value_vertical_squared = pixel_value_vertical * pixel_value_vertical;
            long sum_squarred = pixel_value_horizontal_squared + pixel_value_vertical_squared;

            long pixel_value_corner = pixel_value_horizontal_squared * pixel_value_vertical_squared -
                                      pixel_value_horizontal * pixel_value_vertical -
                                      LAMBDA * (
                                              ((pixel_value_horizontal_squared) * (pixel_value_vertical_squared)) *
                                              (pixel_value_horizontal_squared) * (pixel_value_vertical_squared));
            if (pixel_value_corner > 255) {
                pixel_value_corner = 255;
            }
            targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(pixel_value_corner);
        }
    }
}


void
extractHarrysKeypointsSorted(Mat &frame, Mat &targetFrame, const int horizontal_mask[3][3], const int vertical_mask[3][3]) {
    Mat horizontal_masked_image = targetFrame.clone();
    Mat vertical_masked_image = targetFrame.clone();

    for (int row_index = 1; row_index < (frame.rows - 1); row_index++) {
        for (int col_index = 1; col_index < (frame.cols - 1); col_index++) {

            int pixel_value_horizontal = abs(
                    horizontal_mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    horizontal_mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    horizontal_mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    horizontal_mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    horizontal_mask[1][1] * frame.at<char>(row_index, col_index) +
                    horizontal_mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    horizontal_mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    horizontal_mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    horizontal_mask[2][2] * frame.at<char>(row_index + 1, col_index + 1));

            int pixel_value_vertical = abs(
                    vertical_mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    vertical_mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    vertical_mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    vertical_mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    vertical_mask[1][1] * frame.at<char>(row_index, col_index) +
                    vertical_mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    vertical_mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    vertical_mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    vertical_mask[2][2] * frame.at<char>(row_index + 1, col_index + 1));

            pixel_value_horizontal >= SEUIL ? pixel_value_horizontal = pixel_value_horizontal
                                            : pixel_value_horizontal = 0;

            pixel_value_vertical >= SEUIL ? pixel_value_vertical = pixel_value_vertical
                                          : pixel_value_vertical = 0;
            if (pixel_value_horizontal > 255)
                pixel_value_horizontal = 255;
            if (pixel_value_vertical > 255)
                pixel_value_vertical = 255;

            long pixel_value_horizontal_squared = pixel_value_horizontal * pixel_value_horizontal;
            long pixel_value_vertical_squared = pixel_value_vertical * pixel_value_vertical;
            long sum_squarred = pixel_value_horizontal_squared + pixel_value_vertical_squared;

            long pixel_value_corner = pixel_value_horizontal_squared * pixel_value_vertical_squared -
                                      pixel_value_horizontal * pixel_value_vertical -
                                      LAMBDA * (
                                              ((pixel_value_horizontal_squared) * (pixel_value_vertical_squared)) *
                                              (pixel_value_horizontal_squared) * (pixel_value_vertical_squared));
            if (pixel_value_corner > 255) {
                pixel_value_corner = 255;
            }
            targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(pixel_value_corner);
        }
    }
}


int run_harrys() {
    Vec3f pixel;


    VideoCapture cap(1); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
        return -1;

    Mat horizontal_masked_image, frame, greyscale_image, vertical_masked_image, harrys_image, gaussian_image, gaussian_image2;
    namedWindow("GogolCam", 1);
    namedWindow("Mask", 1);
    namedWindow("GaussianBlur", 1);

    moveWindow("Mask", 0, 0);
    moveWindow("GogolCam", 700, 0);
    moveWindow("GaussianBlur", 0, 400);


    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);  //taille de la fenetre
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480); //au dela de 320*240

    MatIterator_<Vec3b> it, end;

    int id = 0;

    while (true) {
        int sumX = 0, sumY = 0, nbPixels = 0;
        if (cap.read(frame)) {// get a new frame from camera

            cvtColor(frame, greyscale_image, CV_RGB2GRAY);
            harrys_image = greyscale_image.clone();
            gaussian_image = greyscale_image.clone();
//            gaussian_image2 = greyscale_image.clone();

//            gaussianBlur(greyscale_image,gaussian_image,gaussian_mask);

            extractHarrysKeypoints(greyscale_image, harrys_image, horizontal_mask, vertical_mask);

            imshow("Mask", harrys_image);
            imshow("GogolCam", frame);
//            imshow("GaussianBlur", gaussian_image);

        }
        if (waitKey(30) >= 0)
            break;
    }
    destroyAllWindows();
    return 0;
}

int main() {
    run_harrys();
//
//    std::list<pixel> pixelList;
//
//    pixel p1 = std::make_tuple(8, 2, 3);
//    pixel p2 = std::make_tuple(3, 2, 3);
//    pixel p3 = std::make_tuple(9, 2, 3);
//
//    pixelList.push_back(p1);
//    pixelList.push_back(p2);
//    pixelList.push_back(p3);
//
//    pixelList.sort([](const pixel &pixel1, const pixel &pixel2) {
//        return std::get<0>(pixel1) > std::get<0>(pixel2);
//    });
//
//    cout << std::get<0>(pixelList.front()) << endl;

    return 0;
}