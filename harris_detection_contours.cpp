#include <opencv2/opencv.hpp>
#include <fstream>

#define B 0
#define G 1
#define R 2
#define CONTRAST 75

using namespace cv;
using namespace std;

int vertical_mask[3][3] = {{-1, 0, 1},
                           {-1, 0, 1},
                           {-1, 0, 1}};

int horizontal_mask[3][3] = {{-1, -1, -1},
                             {0,  0,  0},
                             {1,  1,  1}};


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


void extractHarrysKeypoints(Mat &frame, Mat &targetFrame, const int horizontal_mask[3][3], const int vertical_mask[3][3]) {
    Mat horizontal_masked_image = targetFrame.clone();
    Mat vertical_masked_image = targetFrame.clone();
    for (int row_index = 1; row_index < (frame.rows - 1); row_index++) {
        for (int col_index = 1; col_index < (frame.cols - 1); col_index++) {
            int pixel_value_horizontal = floor(
                    horizontal_mask[0][0] * frame.at<uchar>(row_index - 1, col_index - 1) +
                    horizontal_mask[0][1] * frame.at<uchar>(row_index - 1, col_index) +
                    horizontal_mask[0][2] * frame.at<uchar>(row_index - 1, col_index + 1) +
                    horizontal_mask[1][0] * frame.at<uchar>(row_index, col_index - 1) +
                    horizontal_mask[1][1] * frame.at<uchar>(row_index, col_index) +
                    horizontal_mask[1][2] * frame.at<uchar>(row_index, col_index + 1) +
                    horizontal_mask[2][0] * frame.at<uchar>(row_index + 1, col_index - 1) +
                    horizontal_mask[2][1] * frame.at<uchar>(row_index + 1, col_index) +
                    horizontal_mask[2][2] * frame.at<uchar>(row_index + 1, col_index + 1) / 9);

            int pixel_value_vertical = floor(
                    vertical_mask[0][0] * frame.at<uchar>(row_index - 1, col_index - 1) +
                    vertical_mask[0][1] * frame.at<uchar>(row_index - 1, col_index) +
                    vertical_mask[0][2] * frame.at<uchar>(row_index - 1, col_index + 1) +
                    vertical_mask[1][0] * frame.at<uchar>(row_index, col_index - 1) +
                    vertical_mask[1][1] * frame.at<uchar>(row_index, col_index) +
                    vertical_mask[1][2] * frame.at<uchar>(row_index, col_index + 1) +
                    vertical_mask[2][0] * frame.at<uchar>(row_index + 1, col_index - 1) +
                    vertical_mask[2][1] * frame.at<uchar>(row_index + 1, col_index) +
                    vertical_mask[2][2] * frame.at<uchar>(row_index + 1, col_index + 1) / 9);

            // TODO ADD THRESHOLD HERE
            targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(pixel_value_horizontal *
                                                                             pixel_value_vertical);

        }
    }

}


int run_harrys() {
    Vec3f pixel;


    VideoCapture cap(1); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
        return -1;

    Mat horizontal_masked_image, frame, greyscale_image, vertical_masked_image, harrys_image;
    namedWindow("GogolCam", 1);
    namedWindow("Mask", 1);
    moveWindow("Mask", 0, 0);
    moveWindow("GogolCam", 700, 0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);  //taille de la fenetre
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480); //au dela de 320*240

    MatIterator_<Vec3b> it, end;

    int id = 0;

    while (true) {
        int sumX = 0, sumY = 0, nbPixels = 0;
        if (cap.read(frame)) {// get a new frame from camera

            cvtColor(frame, greyscale_image, CV_RGB2GRAY);
            harrys_image = greyscale_image.clone();
//            horizontal_masked_image = greyscale_image.clone();
//            vertical_masked_image = greyscale_image.clone();
//            applyMatrix(greyscale_image, horizontal_masked_image, horizontal_mask);
//            applyMatrix(greyscale_image, vertical_masked_image, vertical_mask);
            //cout << "Current FPS : " << cap.get(CV_CAP_PROP_FPS);
            extractHarrysKeypoints(greyscale_image, harrys_image, horizontal_mask, vertical_mask);
            imshow("Mask", harrys_image);
            imshow("GogolCam", frame);
        }
        if (waitKey(30) >= 0)
            break;
    }
    destroyAllWindows();
    return 0;
}

int main() {
    run_harrys();
    return 0;
}