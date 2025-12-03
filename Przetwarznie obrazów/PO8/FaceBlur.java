import org.opencv.core.*;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;
import org.opencv.videoio.VideoCapture;

import javax.swing.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;

public class FaceBlur {

    public static void main(String[] args) {
        // 1. Załaduj bibliotekę natywną OpenCV
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);

        // 2. Załaduj klasyfikator do wykrywania twarzy
        // UPEWNIJ SIĘ, ŻE ŚCIEŻKA DO PLIKU XML JEST POPRAWNA!
        CascadeClassifier faceDetector = new CascadeClassifier("haarcascade_frontalface_alt.xml");

        // 3. Uruchom kamerę (0 to zazwyczaj domyślna kamera laptopa)
        VideoCapture camera = new VideoCapture(0);

        if (!camera.isOpened()) {
            System.out.println("Błąd: Nie można otworzyć kamery.");
            return;
        }

        // 4. Przygotuj okno do wyświetlania
        JFrame frame = new JFrame("Face Blur Live");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 600);
        JLabel imageLabel = new JLabel();
        frame.add(imageLabel);
        frame.setVisible(true);

        Mat matrix = new Mat();
        MatOfRect faceDetections = new MatOfRect();

        // 5. Główna pętla przetwarzania
        while (true) {
            // Pobierz klatkę z kamery
            camera.read(matrix);

            if (!matrix.empty()) {
                // Wykryj twarze
                faceDetector.detectMultiScale(matrix, faceDetections);

                // Dla każdej wykrytej twarzy...
                for (Rect rect : faceDetections.toArray()) {
                    // Wybierz obszar twarzy (Region of Interest)
                    Mat faceROI = matrix.submat(rect);

                    // Zastosuj rozmycie Gaussa (Blur)
                    // Parametr Size(45, 45) określa siłę rozmycia (musi być nieparzysty)
                    Imgproc.GaussianBlur(faceROI, faceROI, new Size(55, 55), 55);
                }

                // Konwersja Mat do BufferedImage i wyświetlenie
                ImageIcon imageIcon = new ImageIcon(matToBufferedImage(matrix));
                imageLabel.setIcon(imageIcon);
                frame.pack(); // Dopasuj okno do rozmiaru obrazu
            }
        }
    }

    // Metoda pomocnicza do konwersji formatu OpenCV (Mat) na format Java Swing (BufferedImage)
    public static BufferedImage matToBufferedImage(Mat m) {
        int type = BufferedImage.TYPE_BYTE_GRAY;
        if (m.channels() > 1) {
            type = BufferedImage.TYPE_3BYTE_BGR;
        }
        int bufferSize = m.channels() * m.cols() * m.rows();
        byte[] b = new byte[bufferSize];
        m.get(0, 0, b); // Pobierz wszystkie piksele
        BufferedImage image = new BufferedImage(m.cols(), m.rows(), type);
        final byte[] targetPixels = ((DataBufferByte) image.getRaster().getDataBuffer()).getData();
        System.arraycopy(b, 0, targetPixels, 0, b.length);
        return image;
    }
}