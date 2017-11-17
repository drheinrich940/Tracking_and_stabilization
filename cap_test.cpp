#include <opencv2/opencv.hpp>

#define B 0
#define G 1
#define R 2
#define CONTRAST 65

using namespace cv;

int main(int, char **) {

    Vec3f pixel;

    VideoCapture cap(1); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
        return -1;

    Mat mask, frame;
    namedWindow("GogolCam", 1);
    namedWindow("Mask", 1);
    moveWindow("Mask", 0, 0);
    moveWindow("GogolCam", 700, 0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);  //taille de la fenetre
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480); //au dela de 320*240

    MatIterator_<Vec3b> it, end;


    while (true) {
        int sumX = 0, sumY = 0, nbPixels = 0;
        if (cap.read(frame)) {// get a new frame from camera

            mask = frame.clone();
            Point *center = new Point(frame.cols / 2, frame.rows / 2);
            for (it = mask.begin<Vec3b>(), end = mask.end<Vec3b>(); it != end; ++it) {
                if ((((*it)[R]) - ((*it)[B])) > CONTRAST && (((*it)[R]) - (*it)[G]) > CONTRAST) {
                    (*it)[0] = 255; // accès au B
                    (*it)[1] = 255; // le G
                    (*it)[2] = 255; // le R
                    sumX += it.pos().x;
                    sumY += it.pos().y;
                    nbPixels++;
                } else {
                    (*it)[0] = 0; // accès au B
                    (*it)[1] = 0; // le G
                    (*it)[2] = 0; // le R
                }
            }

            Point *barycentre = nullptr;
            if (nbPixels > 30) {
                barycentre = new Point(sumX / nbPixels, sumY / nbPixels);
                circle(frame, *barycentre, 3, CV_RGB(0, 0, 255), 5);
            }

            if (barycentre != nullptr) {
                int diffX, diffY;
                diffX = center->x - barycentre->x;
                diffY = center->y - barycentre->y;
                if (diffX > 20) {
                    printf("%c", 'A' + diffX/10);
                } else if (diffX < -20) {
                    printf("%c", 'a' - diffX/10);
                } else
                    std::cout << "0";

                if (diffY > 20) {
                    printf("%c\n", 'A' + diffY / 10);
                } else if (diffY < -20) {
                    printf("%c\n", 'a' - diffY / 10);
                } else
                    std::cout << "0" << std::endl;
            }

            imshow("Mask", mask);
            imshow("GogolCam", frame);
        }
        if (waitKey(30) >= 0)
            break;
    }
    destroyAllWindows();
    return 0;
}
