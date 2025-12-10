module com.example.programowanie_3 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;


    opens com.example.programowanie_3 to javafx.fxml;
    exports com.example.programowanie_3;
}