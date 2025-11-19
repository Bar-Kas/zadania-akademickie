import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Scalar;
import org.opencv.core.Point;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;
import org.opencv.highgui.HighGui;
import java.util.List;
import java.util.ArrayList;

public class Zad5_Contours {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        Mat img = Imgcodecs.imread("krztalty.png");
        if (img.empty()) {
            System.err.println("Nie można wczytać obrazu: kruszek.jpg");
            System.exit(1);
        }


        Mat gray = new Mat();
        Imgproc.cvtColor(img, gray, Imgproc.COLOR_BGR2GRAY);
        Imgproc.GaussianBlur(gray, gray, new Size(5, 5), 1.5);

        Mat binary = new Mat();
        Imgproc.threshold(gray, binary, 100, 255, Imgproc.THRESH_BINARY);

        // Znajdź kontury
        List<MatOfPoint> contours = new ArrayList<>();
        Mat hierarchy = new Mat();
        Imgproc.findContours(binary, contours, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

        Mat drawing = img.clone();
        Scalar color = new Scalar(0, 0, 255);
        Imgproc.drawContours(drawing, contours, -1, color, 2);

        for (int i = 0; i < contours.size(); i++) {
            org.opencv.core.Rect r = Imgproc.boundingRect(contours.get(i));
            Imgproc.rectangle(drawing, new Point(r.x, r.y), new Point(r.x + r.width, r.y + r.height), new Scalar(0,255,0), 1);
        }

        HighGui.imshow("Zadanie 5 - Kontury", drawing);
        HighGui.imshow("Zadanie 5 - Binary", binary);
        HighGui.waitKey(0);
        System.exit(0);
    }
}
