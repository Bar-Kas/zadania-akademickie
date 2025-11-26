import org.opencv.core.*;
import org.opencv.highgui.HighGui;
import org.opencv.imgproc.Imgproc;
import org.opencv.videoio.VideoCapture;
import org.opencv.objdetect.CascadeClassifier;

public class zad9 {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        VideoCapture capture = new VideoCapture(0);

        if (!capture.isOpened()) {
            System.err.println("Blad kamery.");
            System.exit(1);
        }

        CascadeClassifier faceCascade = new CascadeClassifier("haarcascade_frontalface_default.xml");

        Mat frame = new Mat();
        Mat gray = new Mat();
        MatOfRect faces = new MatOfRect();

        while (capture.read(frame)) {
            Imgproc.cvtColor(frame, gray, Imgproc.COLOR_BGR2GRAY);
            Imgproc.equalizeHist(gray, gray);

            faceCascade.detectMultiScale(gray, faces);

            for (Rect rect : faces.toArray()) {
                Imgproc.rectangle(frame, new Point(rect.x, rect.y),
                        new Point(rect.x + rect.width, rect.y + rect.height),
                        new Scalar(255, 0, 0), 2);
            }

            HighGui.imshow("Zad 9", frame);
            if (HighGui.waitKey(10) == 27) break;
        }

        capture.release();
        System.exit(0);
    }
}