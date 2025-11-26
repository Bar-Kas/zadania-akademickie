import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.highgui.HighGui;
import org.opencv.imgproc.Imgproc;
import org.opencv.videoio.VideoCapture;

public class zad6 {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        VideoCapture capture = new VideoCapture(0);
        Mat background = Imgcodecs.imread("kruszek.jpg");
        Mat frameImg = Imgcodecs.imread("ramka.png");

        int margin = 10;
        int innerWidth = frameImg.cols() - (2 * margin);
        int innerHeight = frameImg.rows() - (2 * margin);

        Mat frame = new Mat();
        Mat hsv = new Mat();
        Mat mask = new Mat();
        Mat result = new Mat();
        Mat bgResized = new Mat();
        Mat resultResized = new Mat();

        while (capture.read(frame)) {
            Imgproc.resize(background, bgResized, new Size(frame.width(), frame.height()));
            Imgproc.cvtColor(frame, hsv, Imgproc.COLOR_BGR2HSV);
            Core.inRange(hsv, new Scalar(35, 50, 50), new Scalar(85, 255, 255), mask);

            int greenPixels = Core.countNonZero(mask);

            if (greenPixels < (frame.total() * 0.01)) {
                Imgproc.putText(frame, "No greenscreen", new Point(50, 50),
                        Imgproc.FONT_HERSHEY_SIMPLEX, 1.0, new Scalar(0, 0, 255), 2);
                result = frame;
            } else {
                Mat kernel = Imgproc.getStructuringElement(Imgproc.MORPH_ELLIPSE, new Size(3, 3));
                Imgproc.morphologyEx(mask, mask, Imgproc.MORPH_OPEN, kernel);
                Mat maskInv = new Mat();
                Core.bitwise_not(mask, maskInv);
                Mat person = new Mat();
                Core.bitwise_and(frame, frame, person, maskInv);
                Mat bgCropped = new Mat();
                Core.bitwise_and(bgResized, bgResized, bgCropped, mask);
                Core.add(person, bgCropped, result);
            }

            if (innerWidth > 0 && innerHeight > 0) {
                Imgproc.resize(result, resultResized, new Size(innerWidth, innerHeight));
                Mat roi = frameImg.submat(new Rect(margin, margin, resultResized.cols(), resultResized.rows()));
                resultResized.copyTo(roi);
            }

            HighGui.imshow("Zadanie 6 - OBS Check", frameImg);
            if (HighGui.waitKey(10) == 27) break;
        }

        capture.release();
        System.exit(0);
    }
}