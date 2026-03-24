module com.example.programowanie9 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;


    opens com.example.programowanie9 to javafx.fxml;
    exports com.example.programowanie9;
}