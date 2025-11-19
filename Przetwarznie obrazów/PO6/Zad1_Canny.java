import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;
import org.opencv.highgui.HighGui;

public class Zad1_Canny {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        Mat img = Imgcodecs.imread("kruszek.jpg");
        if (img.empty()) {
            System.err.println("Nie można wczytać obrazu: kruszek.jpg");
            System.exit(1);
        }

        Mat gray = new Mat();
        Imgproc.cvtColor(img, gray, Imgproc.COLOR_BGR2GRAY);
        Imgproc.GaussianBlur(gray, gray, new Size(5, 5), 1.5);

        double threshold1 = 50;
        double threshold2 = 150;
        Mat edges = new Mat();
        Imgproc.Canny(gray, edges, threshold1, threshold2);

        HighGui.imshow("Zadanie 1 - Canny (edges)", edges);
        HighGui.waitKey(0);
        System.exit(0);
    }
}
