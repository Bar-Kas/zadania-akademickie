import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.highgui.HighGui;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;

public class zad7 {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        Mat img = Imgcodecs.imread("face.jpg");
        if (img.empty()) { System.exit(1); }

        CascadeClassifier faceDetector = new CascadeClassifier("haarcascade_frontalface_default.xml");

        Mat gray = new Mat();
        Imgproc.cvtColor(img, gray, Imgproc.COLOR_BGR2GRAY);
        Imgproc.equalizeHist(gray, gray);

        MatOfRect faces = new MatOfRect();
        faceDetector.detectMultiScale(gray, faces, 1.1, 3);

        for (Rect rect : faces.toArray()) {
            Imgproc.rectangle(img, new Point(rect.x, rect.y),
                    new Point(rect.x + rect.width, rect.y + rect.height),
                    new Scalar(0, 255, 0), 2);
        }

        HighGui.imshow("Wykryta twarz", img);
        HighGui.waitKey(0);
        System.exit(0);
    }
}