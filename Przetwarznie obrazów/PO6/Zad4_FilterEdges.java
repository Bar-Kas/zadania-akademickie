import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;
import org.opencv.highgui.HighGui;

public class Zad4_FilterEdges {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        Mat img = Imgcodecs.imread("kruszek.jpg");
        if (img.empty()) {
            System.err.println("Nie można wczytać obrazu: kruszek.jpg");
            System.exit(1);
        }

        Mat gray = new Mat();
        Imgproc.cvtColor(img, gray, Imgproc.COLOR_BGR2GRAY);
        Mat edgesRaw = new Mat();
        Imgproc.Canny(gray, edgesRaw, 15, 100);

        Mat blurred = new Mat();
        Imgproc.GaussianBlur(gray, blurred, new Size(7, 7), 2.0);
        Mat edgesBlurred = new Mat();
        Imgproc.Canny(blurred, edgesBlurred, 80, 200);

        Mat morph = new Mat();
        Mat kernel = Imgproc.getStructuringElement(Imgproc.MORPH_RECT, new Size(3, 3));
        Imgproc.morphologyEx(edgesBlurred, morph, Imgproc.MORPH_OPEN, kernel);

        HighGui.imshow("Zadanie 4 - edges (raw)", edgesRaw);
        HighGui.imshow("Zadanie 4 - edges (blurred)", edgesBlurred);
        HighGui.imshow("Zadanie 4 - edges (after morphology)", morph);
        HighGui.waitKey(0);
        System.exit(0);
    }
}
