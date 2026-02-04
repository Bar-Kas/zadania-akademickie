module com.example.programowanie8 {
    requires javafx.controls;
    requires javafx.fxml;


    opens com.example.programowanie8 to javafx.fxml;
    exports com.example.programowanie8;
}