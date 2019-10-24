#include <opencv2/opencv.hpp>
#include <fstream>

#define B 0
#define G 1
#define R 2
#define CONTRAST 75
#define SEUIL 20
#define LAMBDA 3
#define PIXEL_AMOUNT 100000
using namespace cv;
using namespace std;

// TODO : REPLACE FUCKING LIST WITH ARRAY


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


/**
 * Meaning in time of the image
 * @param previousFrame
 * @param currentFrame
 * @param targetFrame
 */
void meaningImageInTime(Mat &previousFrame, Mat &currentFrame, Mat &targetFrame) {
    for (int row_index = 0; row_index < (previousFrame.rows); row_index++) {
        for (int col_index = 0; col_index < (previousFrame.cols); col_index++) {


            double value = ((0.1 *
                             static_cast<double>(previousFrame.at<char>(row_index,
                                                                        col_index)) +
                             (0.9 *
                              static_cast<double>(currentFrame.at<char>(row_index,
                                                                        col_index)))));
            if (value > 255)
                value = 255;
            targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(value);


        }
    }
}

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
extractHarrysKeypoints(Mat &frame, Mat &targetFrame, const int h_mask[3][3], const int v_mask[3][3]) {
    Mat horizontal_masked_image = targetFrame.clone();
    Mat vertical_masked_image = targetFrame.clone();

    for (int row_index = 1; row_index < (frame.rows - 1); row_index++) {
        for (int col_index = 1; col_index < (frame.cols - 1); col_index++) {

            int pixel_value_horizontal = abs(
                    h_mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    h_mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    h_mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    h_mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    h_mask[1][1] * frame.at<char>(row_index, col_index) +
                    h_mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    h_mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    h_mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    h_mask[2][2] * frame.at<char>(row_index + 1, col_index + 1));

            int pixel_value_vertical = abs(
                    v_mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    v_mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    v_mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    v_mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    v_mask[1][1] * frame.at<char>(row_index, col_index) +
                    v_mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    v_mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    v_mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    v_mask[2][2] * frame.at<char>(row_index + 1, col_index + 1));

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

            long pixel_value_corner = pixel_value_horizontal_squared * pixel_value_vertical_squared -
                                      pixel_value_horizontal * pixel_value_vertical -
                                      LAMBDA * (
                                              ((pixel_value_horizontal_squared) * (pixel_value_vertical_squared)) *
                                              (pixel_value_horizontal_squared) * (pixel_value_vertical_squared));

            if (pixel_value_corner > 150 * 150) {
                pixel_value_corner = 255;
            }
            targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(pixel_value_corner);
        }
    }
}

int
extractHarrysKeypointsLocation(Mat &frame, Mat &targetFrame, int pixelLocation[PIXEL_AMOUNT][2], const int h_mask[3][3],
                               const int v_mask[3][3]) {
    Mat horizontal_masked_image = targetFrame.clone();
    Mat vertical_masked_image = targetFrame.clone();
    int pixelCounter = 0;

    for (int row_index = 1; row_index < (frame.rows - 1); row_index++) {
        for (int col_index = 1; col_index < (frame.cols - 1); col_index++) {

            int pixel_value_horizontal = abs(
                    h_mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    h_mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    h_mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    h_mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    h_mask[1][1] * frame.at<char>(row_index, col_index) +
                    h_mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    h_mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    h_mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    h_mask[2][2] * frame.at<char>(row_index + 1, col_index + 1));

            int pixel_value_vertical = abs(
                    v_mask[0][0] * frame.at<char>(row_index - 1, col_index - 1) +
                    v_mask[0][1] * frame.at<char>(row_index - 1, col_index) +
                    v_mask[0][2] * frame.at<char>(row_index - 1, col_index + 1) +
                    v_mask[1][0] * frame.at<char>(row_index, col_index - 1) +
                    v_mask[1][1] * frame.at<char>(row_index, col_index) +
                    v_mask[1][2] * frame.at<char>(row_index, col_index + 1) +
                    v_mask[2][0] * frame.at<char>(row_index + 1, col_index - 1) +
                    v_mask[2][1] * frame.at<char>(row_index + 1, col_index) +
                    v_mask[2][2] * frame.at<char>(row_index + 1, col_index + 1));

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

            long pixel_value_corner = pixel_value_horizontal_squared * pixel_value_vertical_squared -
                                      pixel_value_horizontal * pixel_value_vertical -
                                      LAMBDA * (
                                              ((pixel_value_horizontal_squared) * (pixel_value_vertical_squared)) *
                                              (pixel_value_horizontal_squared) * (pixel_value_vertical_squared));

            if (pixel_value_corner > 150 * 150) {
                pixel_value_corner = 255;
            }
            if (pixelCounter < PIXEL_AMOUNT && pixel_value_corner == 255) {
                // In order to display only them
                targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(pixel_value_corner);
                pixelLocation[pixelCounter][0] = row_index;
                pixelLocation[pixelCounter][1] = col_index;

                pixelCounter++;
            }
        }
    }
    return pixelCounter;
}

void substractList(int currentPixelLocation[PIXEL_AMOUNT][2], int previousPixelLocation[PIXEL_AMOUNT][2],
                   int resultPixelLocation[PIXEL_AMOUNT][2]) {

    for (int i = 0; i < PIXEL_AMOUNT; ++i) {
        resultPixelLocation[i][0] = currentPixelLocation[i][0] - previousPixelLocation[i][0];
        resultPixelLocation[i][1] = currentPixelLocation[i][1] - previousPixelLocation[i][1];

    }
}

void meanMovement(int resultPixelLocation[PIXEL_AMOUNT][2], int resultCoordinate[2], int numberOfKeypointsExtracted) {

    double resultX = 0.0;
    double resultY = 0.0;
    for (int i = 0; i < numberOfKeypointsExtracted; ++i) {
        resultX += resultPixelLocation[i][0];
        resultY += resultPixelLocation[i][1];
    }
    resultCoordinate[0] = static_cast<int>(resultX / numberOfKeypointsExtracted);
    resultCoordinate[1] = static_cast<int>(resultY / numberOfKeypointsExtracted);

}

void
extractHarrysKeypointsSorted(Mat &frame, Mat &targetFrame, const int horizontal_mask[3][3],
                             const int vertical_mask[3][3]) {
    Mat horizontal_masked_image = targetFrame.clone();
    Mat vertical_masked_image = targetFrame.clone();
    std::list<pixel> keyPointsList;


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

            long pixel_value_horizontal_squared = pixel_value_horizontal * pixel_value_horizontal;
            long pixel_value_vertical_squared = pixel_value_vertical * pixel_value_vertical;
            long sum_squarred = pixel_value_horizontal_squared + pixel_value_vertical_squared;

//            long pixel_value_corner = pixel_value_horizontal_squared * pixel_value_vertical_squared -
//                                      pixel_value_horizontal * pixel_value_vertical -
//                                      LAMBDA * (
//                                              ((pixel_value_horizontal_squared) * (pixel_value_vertical_squared)) *
//                                              (pixel_value_horizontal_squared) * (pixel_value_vertical_squared));
            long pixel_value_corner = pixel_value_horizontal_squared * pixel_value_vertical_squared;

            pixel currPixel = std::make_tuple(pixel_value_corner, row_index, col_index);
            keyPointsList.push_back(currPixel);

//            targetFrame.at<uchar>(row_index, col_index) = static_cast<uchar>(pixel_value_corner);
        }
    }
    keyPointsList.sort([](const pixel &pixel1, const pixel &pixel2) {
        return std::get<0>(pixel1) > std::get<0>(pixel2);
    });

    for (int i = 0; i < PIXEL_AMOUNT; ++i) {
        pixel current_pixel = keyPointsList.front();
//        int value = std::get<0>(current_pixel);
        int row_coordinate = std::get<1>(current_pixel);
        int col_coordinate = std::get<2>(current_pixel);
        targetFrame.at<uchar>(row_coordinate, col_coordinate) = static_cast<uchar>(255);
        keyPointsList.pop_front();
    }

}

void copyArray(int sourceArray[PIXEL_AMOUNT][2], int targetArray[PIXEL_AMOUNT][2]) {
    for (int i = 0; i < PIXEL_AMOUNT; ++i) {
        targetArray[i][0] = sourceArray[i][0];
        targetArray[i][1] = sourceArray[i][1];
    }
}

int run_harrys2() {
    Vec3f pixel;

    VideoCapture cap(1); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
        return -1;

    Mat horizontal_masked_image, frame, greyscale_image, vertical_masked_image, harrys_image;
    int previousPixelLocationTab[PIXEL_AMOUNT][2] = {{0}};
    int currentPixelLocationTab[PIXEL_AMOUNT][2] = {{0}};
    namedWindow("GogolCam", 1);
    namedWindow("Mask", 1);
    moveWindow("Mask", 0, 0);
    moveWindow("GogolCam", 700, 0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);  //taille de la fenetre
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480); //au dela de 320*240

    MatIterator_<Vec3b> it, end;
    if (cap.read(frame)) {// get a new frame from camera

        cvtColor(frame, greyscale_image, CV_RGB2GRAY);

        Mat result_image(frame.rows, frame.cols, CV_8UC1, Scalar(0));
        extractHarrysKeypointsLocation(greyscale_image, result_image, previousPixelLocationTab, horizontal_mask,
                                       vertical_mask);

        while (true) {
            if (cap.read(frame)) {// get a new frame from camera
                int resultMovementCoordinate[2] = {0};
                int resultPixelLocationTab[PIXEL_AMOUNT][2] = {{0}};

                cvtColor(frame, greyscale_image, CV_RGB2GRAY);

                Mat result_image2(frame.rows, frame.cols, CV_8UC1, Scalar(0));
                int numberOfKeypointsExtracted = extractHarrysKeypointsLocation(greyscale_image, result_image2,
                                                                                currentPixelLocationTab,
                                                                                horizontal_mask,
                                                                                vertical_mask);
                substractList(currentPixelLocationTab, previousPixelLocationTab, resultPixelLocationTab);
                copyArray(previousPixelLocationTab, currentPixelLocationTab);
                meanMovement(resultPixelLocationTab, resultMovementCoordinate, numberOfKeypointsExtracted);
                cout << "X : " << resultMovementCoordinate[0] << " Y : " << resultMovementCoordinate[1]
                     << " Number of keypoints extracted : " << numberOfKeypointsExtracted << endl;
                imshow("Mask", result_image2);
                imshow("GogolCam", frame);
//            imshow("GaussianBlur", gaussian_image);

            }
            if (waitKey(30) >= 0)
                break;
        }
    }
    destroyAllWindows();
    return 0;


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


    while (true) {
        if (cap.read(frame)) {// get a new frame from camera

            cvtColor(frame, greyscale_image, CV_RGB2GRAY);

            Mat result_image(frame.rows, frame.cols, CV_8UC1, Scalar(0));
            extractHarrysKeypoints(greyscale_image, result_image, horizontal_mask, vertical_mask);

            imshow("Mask", result_image);
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
    run_harrys2();

    std::list<pixel> pixelList;
//
//    pixel p1 = std::make_tuple(10, 2, 3);
//    pixel p2 = std::make_tuple(3, 2, 3);
//    pixel p3 = std::make_tuple(9, 2, 3);
//
//    pixelList.push_back(p1);
//    p1 = std::make_tuple(20, 2, 3);
//    pixelList.push_back(p1);
//    p1 = std::make_tuple(3, 2, 3);
//
//    pixelList.push_back(p1);
//
//    pixelList.sort([](const pixel &pixel1, const pixel &pixel2) {
//        return std::get<0>(pixel1) > std::get<0>(pixel2);
//    });
//
//    cout << std::get<0>(pixelList.front()) << endl;

    return 0;
}