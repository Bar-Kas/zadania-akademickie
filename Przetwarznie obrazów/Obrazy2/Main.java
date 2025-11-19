// ConvertToHSV.java
import org.opencv.core.*;
        import org.opencv.imgcodecs.*;
        import org.opencv.highgui.*;
        import org.opencv.imgproc.*;

public class Main {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        Mat image = Imgcodecs.imread("kruszek.jpg");

        int screenWidth = 1920, screenHeight = 1080;
        double scale = Math.min((double)screenWidth / image.width(), (double)screenHeight / image.height());
        Mat resized = new Mat();
        Imgproc.resize(image, resized, new Size(image.width() * scale, image.height() * scale));

        Mat hsv = new Mat();
        Imgproc.cvtColor(resized, hsv, Imgproc.COLOR_BGR2HSV);
        HighGui.imshow("HSV", hsv);


        Mat back = new Mat();
        Imgproc.cvtColor(hsv, back, Imgproc.COLOR_HSV2BGR);
        HighGui.imshow("HSV na BGR", back);
        HighGui.waitKey(0);

        System.exit(0);
    }
}
