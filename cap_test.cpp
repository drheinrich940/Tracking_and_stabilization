#include <opencv2/opencv.hpp>
#include <fstream>

#define B 0
#define G 1
#define R 2
#define CONTRAST 75

using namespace cv;
using namespace std;

void ecrire(const string &filename, const string &text) {
    ofstream file;
    file.open(filename, ios_base::out | ios_base::app);
    if (!file.is_open()) {
        cerr << "Impossible d'ouvrir le fichier" << endl;
        exit(1);
    }
    file << text;
    file.flush();
    file.close();
}

int run() {
    Vec3f pixel;

    ofstream stream;
    stream.open("/dev/ttyACM0"/*, ofstream::out*/);

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

    int id = 0;

    while (true) {
        int sumX = 0, sumY = 0, nbPixels = 0;
        if (cap.read(frame)) {// get a new frame from camera

            mask = frame.clone();
            auto *center = new Point(frame.cols / 2, frame.rows / 2);
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
                double hypotenuse;
                diffX = center->x - barycentre->x;
                diffY = center->y - barycentre->y;
                hypotenuse = sqrt(diffX * diffX + diffY * diffY);
                ecrire("../joseboveDist", to_string(hypotenuse) + "," + to_string(id) + "\n");
                // TODO Utiliser une valeur absolue et utiliser un define
                if (diffX > 20 || diffX < -20) {    // Si l'écart horizontal entre le centre et le barycentre > 20
                    auto c = (char) ('L' - diffX / 25);
                    cout << c;
                    stream << c;
                } else {
                    cout << "L";
                    stream << "L";
                }

                if (diffY > 10 || diffY < -10) {
                    auto c = (char) ('l' + diffY / 25);
                    cout << c;
                    stream << c;
                } else {
                    stream << "l";
                    cout << "l";
                }
                stream.flush();
                cout << endl;
            }

            imshow("Mask", mask);
            imshow("GogolCam", frame);
        }
        if (waitKey(30) >= 0)
            break;
    }
    stream.close();
    destroyAllWindows();
    return 0;
}

int main() {
    run();
    return 0;
}