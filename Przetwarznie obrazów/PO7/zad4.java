import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.highgui.HighGui;
import org.opencv.imgproc.Imgproc;

public class zad4 {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        Mat background = Imgcodecs.imread("kruszek.jpg");
        Mat selfie = Imgcodecs.imread("greenscrean.jpg");
        Mat frame = Imgcodecs.imread("ramka.png");

        if (background.empty() || selfie.empty() || frame.empty()) {
            System.exit(1);
        }

        Imgproc.resize(background, background, new Size(selfie.width(), selfie.height()));

        Mat hsv = new Mat();
        Imgproc.cvtColor(selfie, hsv, Imgproc.COLOR_BGR2HSV);

        Mat mask = new Mat();
        Core.inRange(hsv, new Scalar(35, 50, 50), new Scalar(85, 255, 255), mask);

        Mat kernel = Imgproc.getStructuringElement(Imgproc.MORPH_ELLIPSE, new Size(5, 5));
        Imgproc.morphologyEx(mask, mask, Imgproc.MORPH_OPEN, kernel);
        Imgproc.morphologyEx(mask, mask, Imgproc.MORPH_CLOSE, kernel);

        Mat maskInv = new Mat();
        Core.bitwise_not(mask, maskInv);

        Mat person = new Mat();
        Core.bitwise_and(selfie, selfie, person, maskInv);
        Mat bgCropped = new Mat();
        Core.bitwise_and(background, background, bgCropped, mask);
        Mat result = new Mat();
        Core.add(person, bgCropped, result);

        int margin = 10;
        int innerWidth = frame.cols() - (2 * margin);
        int innerHeight = frame.rows() - (2 * margin);

        if (innerWidth <= 0 || innerHeight <= 0) {
            System.exit(1);
        }

        Mat resultResized = new Mat();
        Imgproc.resize(result, resultResized, new Size(innerWidth, innerHeight));

        Mat roi = frame.submat(new Rect(margin, margin, resultResized.cols(), resultResized.rows()));
        resultResized.copyTo(roi);

        HighGui.imshow("Wynik margines 10", frame);
        HighGui.waitKey(0);
        System.exit(0);
    }
}