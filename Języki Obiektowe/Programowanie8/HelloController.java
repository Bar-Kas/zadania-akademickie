package com.example.programowanie8;

import javafx.fxml.FXML;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.TextField;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class HelloController {

    @FXML
    private TextField imieField;

    @FXML
    private TextField nazwiskoField;

    @FXML
    private TextField wiekField;

    @FXML
    private ChoiceBox<String> sortBox;

    private ArrayList<Pacjent> patientList = new ArrayList<>();

    @FXML
    public void initialize() {
        sortBox.getItems().addAll(
                "Id",
                "Nazwiska",
                "Imienia",
                "Wieku"
        );
        sortBox.setValue("ID");
    }

    @FXML
    protected void onClickAdd() {
        try {
            String imie = imieField.getText();
            String nazwisko = nazwiskoField.getText();
            String wiekText = wiekField.getText();

            if (imie.isEmpty() || nazwisko.isEmpty() || wiekText.isEmpty()) {
                showError("Wypełnij wszystkie pola!");
                return;
            }

            int wiek = Integer.parseInt(wiekText);

            Pacjent nowyPacjent = new Pacjent(imie, nazwisko, wiek);
            patientList.add(nowyPacjent);
            
            imieField.clear();
            nazwiskoField.clear();
            wiekField.clear();

            System.out.println("Dodano pacjenta: " + imie + " " + nazwisko);

        } catch (NumberFormatException e) {
            showError("Wiek musi być liczbą całkowitą!");
        }
    }

    @FXML
    protected void onClickLog() {
        if (patientList.isEmpty()) {
            System.out.println("Lista jest pusta.");
            return;
        }

        List<Pacjent> toPrint = new ArrayList<>(patientList);
        String kryterium = sortBox.getValue();

        switch (kryterium) {
            case "Nazwiska":
                toPrint.sort(Comparator.comparing(Pacjent::getNazwisko));
                break;
            case "Imienia":
                toPrint.sort(Comparator.comparing(Pacjent::getImie));
                break;
            case "Wieku":
                toPrint.sort(Comparator.comparingInt(Pacjent::getWiek));
                break;
            case "Kolejności dodania":
                toPrint.sort(Comparator.comparing(Pacjent::getId));
            default:
                break;
        }

        for (Pacjent p : toPrint) {
            printAsJson(p);
        }
    }

    private void printAsJson(Pacjent p) {
        System.out.println("{");
        System.out.println("    \"ID\": \"" + p.getId() + "\",");
        System.out.println("    \"imie\": \"" + p.getImie() + "\",");
        System.out.println("    \"nazwisko\": \"" + p.getNazwisko() + "\",");
        System.out.println("    \"wiek\": " + p.getWiek());
        System.out.println("}");
    }

    private void showError(String komunikat) {
        Alert alert = new Alert(AlertType.ERROR);
        alert.setTitle("Error");
        alert.setHeaderText(null);
        alert.setContentText(komunikat);
        alert.showAndWait();
    }
}