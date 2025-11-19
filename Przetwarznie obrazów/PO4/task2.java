
import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.highgui.HighGui;
import org.opencv.imgproc.Imgproc;

public class task2 {
    static { System.loadLibrary(Core.NATIVE_LIBRARY_NAME); }

    public static void main(String[] args) {
        String inputFile = "kruszek.jpg";
        Mat src = Imgcodecs.imread(inputFile);
        if (src.empty()) {
            System.err.println("Nie można wczytać pliku: " + inputFile);
            System.exit(1);
        }

        // Konwersja do skali szarości i binaryzacja (Otsu)
        Mat gray = new Mat();
        Imgproc.cvtColor(src, gray, Imgproc.COLOR_BGR2GRAY);
        Mat binary = new Mat();
        Imgproc.threshold(gray, binary, 0, 255, Imgproc.THRESH_BINARY | Imgproc.THRESH_OTSU);
        Imgcodecs.imwrite("binary.png", binary);

        // Parametry: różne kształty i rozmiary elementów strukturalnych
        int[] shapes = { Imgproc.MORPH_RECT, Imgproc.MORPH_ELLIPSE, Imgproc.MORPH_CROSS };
        String[] shapeNames = { "rect", "ellipse", "cross" };
        int[] sizes = { 3, 5, 7 };
        // Różne liczby iteracji erozji
        int[] iterations = { 1, 3, 5, 10 };

        for (int s = 0; s < shapes.length; s++) {
            for (int k : sizes) {
                Mat kernel = Imgproc.getStructuringElement(shapes[s], new Size(k, k));
                for (int it : iterations) {
                    Mat eroded = new Mat();
                    // erozja z określoną liczbą iteracji
                    Imgproc.erode(binary, eroded, kernel, new Point(-1, -1), it);
                    String outName = String.format("erode_%s_%dx%d_it%d.png", shapeNames[s], k, k, it);
                    Imgcodecs.imwrite(outName, eroded);
                }
            }
        }

        // Opcjonalny podgląd
        HighGui.imshow("Original", src);
        HighGui.imshow("Binary (Otsu)", binary);
        HighGui.waitKey(0);
        System.exit(0);
    }
}
