import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.highgui.HighGui;
import org.opencv.imgproc.Imgproc;

public class task1 {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void task1(String[] args) {
        String inputFile = "kruszek.jpg";
        Mat src = Imgcodecs.imread(inputFile);


        Mat gray = new Mat();
        Imgproc.cvtColor(src, gray, Imgproc.COLOR_BGR2GRAY);

        Mat binary = new Mat();
        Imgproc.threshold(gray, binary, 0, 255, Imgproc.THRESH_BINARY | Imgproc.THRESH_OTSU);
        Imgcodecs.imwrite("binary.png", binary);

        int[] shapes = { Imgproc.MORPH_RECT, Imgproc.MORPH_ELLIPSE, Imgproc.MORPH_CROSS };
        String[] shapeNames = { "rect", "ellipse", "cross" };
        int[] sizes = { 3, 5, 7 };

        for (int s = 0; s < shapes.length; s++) {
            for (int k : sizes) {
                Mat kernel = Imgproc.getStructuringElement(shapes[s], new Size(k, k));
                Mat eroded = new Mat();
                Imgproc.erode(binary, eroded, kernel);
                String outName = String.format("erode_%s_%dx%d.png", shapeNames[s], k, k);
                Imgcodecs.imwrite(outName, eroded);
            }
        }

        HighGui.imshow("Original", src);
        HighGui.imshow("Binary (Otsu)", binary);
        HighGui.waitKey(0);
        System.exit(0);
    }
}
