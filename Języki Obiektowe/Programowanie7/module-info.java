module com.example.programowanie7 {
    requires javafx.controls;
    requires javafx.fxml;


    opens com.example.programowanie7 to javafx.fxml;
    exports com.example.programowanie7;
}