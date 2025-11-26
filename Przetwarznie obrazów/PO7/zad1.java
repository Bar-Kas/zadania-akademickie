import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.highgui.HighGui;
import org.opencv.imgproc.Imgproc;

public class zad1 {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        Mat background = Imgcodecs.imread("kruszek.jpg");
        Mat selfie = Imgcodecs.imread("greenscrean.jpg");

        if (background.empty() || selfie.empty()) {
            System.exit(1);
        }

        Imgproc.resize(background, background, new Size(selfie.width(), selfie.height()));

        Mat hsv = new Mat();
        Imgproc.cvtColor(selfie, hsv, Imgproc.COLOR_BGR2HSV);

        Mat mask = new Mat();
        Scalar lowerGreen = new Scalar(35, 50, 50);
        Scalar upperGreen = new Scalar(85, 255, 255);
        Core.inRange(hsv, lowerGreen, upperGreen, mask);

        Mat maskInv = new Mat();
        Core.bitwise_not(mask, maskInv);

        Mat person = new Mat();
        Core.bitwise_and(selfie, selfie, person, maskInv);

        Mat bgCropped = new Mat();
        Core.bitwise_and(background, background, bgCropped, mask);

        Mat result = new Mat();
        Core.add(person, bgCropped, result);

        HighGui.imshow("Maska", mask);
        HighGui.imshow("Wynik", result);
        HighGui.waitKey(0);
        System.exit(0);
    }
}