import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.CvType;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;
import org.opencv.highgui.HighGui;

public class Zad3_Sobel {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        Mat img = Imgcodecs.imread("kruszek.jpg");
        if (img.empty()) {
            System.err.println("Nie można wczytać obrazu: kruszek.jpg");
            System.exit(1);
        }

        Mat gray = new Mat();
        Imgproc.cvtColor(img, gray, Imgproc.COLOR_BGR2GRAY);
        Imgproc.GaussianBlur(gray, gray, new Size(3, 3), 0);

        // Sobel - parametry: dx, dy, ksize
        Mat gradX = new Mat();
        Mat gradY = new Mat();
        Imgproc.Sobel(gray, gradX, CvType.CV_16S, 1, 0, 3, 1, 0);
        Imgproc.Sobel(gray, gradY, CvType.CV_16S, 0, 1, 3, 1, 0);

        Mat absGradX = new Mat();
        Mat absGradY = new Mat();
        Core.convertScaleAbs(gradX, absGradX);
        Core.convertScaleAbs(gradY, absGradY);

        Mat grad = new Mat();
        Core.addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

        HighGui.imshow("Zadanie 3 - Sobel (grad)", grad);
        HighGui.waitKey(0);
        System.exit(0);
    }
}
