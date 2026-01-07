package com.example.programowanie5;

import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import java.util.ArrayList;
import java.util.List;

public class HelloController {


    @FXML
    private Label InvLb;
    @FXML
    private Label ManaLb;

    @FXML
    private TextField manaInput;
    @FXML
    private TextField itemInput;


    private int currentMana = 50;
    private final List<String> inventory = new ArrayList<>();


    @FXML
    public void initialize() {
        updateUI();
    }


    @FXML
    protected void onExecuteActionClick() {
        try {
            boolean actionDone = false;


            if (!manaInput.getText().isEmpty()) {
                int cost = Integer.parseInt(manaInput.getText());
                if (cost > currentMana) {
                    String errortext = String.format("Masz za mało many! Posiadasz: %d, a czar kosztuje: %d", currentMana, cost);
                    throw new InsufficientResourcesException(errortext);
                }
                castSpell(cost);
                manaInput.clear();
                actionDone = true;
            }


            if (!itemInput.getText().isEmpty()) {
                String item = itemInput.getText();
                pickUpItem(item);
                itemInput.clear();
                actionDone = true;
            }

            if(itemInput.getText().isEmpty() && manaInput.getText().isEmpty()) {
                String errortext ="Puste pola";
                throw new NoActionPerformed(errortext);
            }

            if (actionDone) {
                updateUI();
            }

        } catch (GameLogicException e) {
            showAlert(Alert.AlertType.ERROR, "Błąd Gry", e.getMessage());
        } catch (NumberFormatException e) {
            showAlert(Alert.AlertType.ERROR, "Błąd Danych", "Koszt many musi być liczbą!");
        }
    }


    private void updateUI() {

        ManaLb.setText("Aktualna Mana: " + currentMana);


        if (inventory.isEmpty()) {
            InvLb.setText("Rzeczy w ekwipunku: (pusto)");
        } else {

            String itemsText = String.join(", ", inventory);
            InvLb.setText("Rzeczy w ekwipunku: " + itemsText);
        }
    }



    private void castSpell(int cost) throws InsufficientResourcesException {
        currentMana -= cost;
    }

    private void pickUpItem(String item){
        inventory.add(item);
    }


    private void showAlert(Alert.AlertType type, String title, String message) {
        Alert alert = new Alert(type);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }
}



abstract class GameLogicException extends Exception {
    public GameLogicException(String message) { super(message); }
}

class NoActionPerformed extends GameLogicException {
    public NoActionPerformed(String message) {
        super(message);
    }
}

class InsufficientResourcesException extends GameLogicException {
    public InsufficientResourcesException(String message) {
        super(message);
    }
}
