import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.core.CvType;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;
import org.opencv.highgui.HighGui;

public class Zad2_Laplacian {
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

        int ksize = 3;
        Mat lap = new Mat();
        Imgproc.Laplacian(gray, lap, CvType.CV_16S, ksize, 1, 0);
        Mat absLap = new Mat();
        Core.convertScaleAbs(lap, absLap);

        HighGui.imshow("Zadanie 2 - Laplacian", absLap);
        HighGui.waitKey(0);
        System.exit(0);
    }
}
