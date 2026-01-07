module com.example.programowanie5 {
    requires javafx.controls;
    requires javafx.fxml;

    requires com.dlsc.formsfx;

    opens com.example.programowanie5 to javafx.fxml;
    exports com.example.programowanie5;
}