import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.highgui.HighGui;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;


public class Main {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }


    public static void main(String[] args) {
        Mat img = Imgcodecs.imread("kruszek.jpg");
        if (img.empty()) { System.err.println("Nie można wczytać obrazu"); System.exit(1); }


        Mat g5 = new Mat();
        Mat g15 = new Mat();


        Imgproc.GaussianBlur(img, g5, new org.opencv.core.Size(7,7), 0);
        Imgproc.GaussianBlur(img, g15, new org.opencv.core.Size(15,15), 0);


        HighGui.imshow("Original", img);
        HighGui.imshow("Gaussian 5x5", g5);
        HighGui.imshow("Gaussian 15x15", g15);


        HighGui.waitKey(0);
        System.exit(0);
    }
}