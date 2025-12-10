package com.example.programowanie_3;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;



public class HelloController {
    @FXML
    private Label topLabel;
    private boolean turn = false ;
    private int turnCount = 0;
    @FXML
    private Button b1,b2,b3,b4,b5,b6,b7,b8,b9;
    private Button[] buttons;

    @FXML
    private void initialize() {
        buttons = new Button[] { b1, b2, b3, b4, b5, b6, b7, b8, b9 };

    }




    private void buttonDisabler(){
        for (Button b : buttons) {
            b.setDisable(true);
        }
    }

    private void buttonReset(){
        for (Button b : buttons) {
            b.setDisable(false);
            b.setText("");
        }
    }

    private String same(Button a, Button b, Button c) {
        String ta = a.getText();
        if (ta == null || ta.isEmpty()) return null;
        if (ta.equals(b.getText()) && ta.equals(c.getText())) return ta;
        return null;
    }

    private String checkWinner() {
        String winner;
        if ((winner = same(b1, b2, b3)) != null) return winner;
        if ((winner = same(b4, b5, b6)) != null) return winner;
        if ((winner = same(b7, b8, b9)) != null) return winner;

        if ((winner = same(b1, b4, b7)) != null) return winner;
        if ((winner = same(b2, b5, b8)) != null) return winner;
        if ((winner = same(b3, b6, b9)) != null) return winner;

        if ((winner = same(b1, b5, b9)) != null) return winner;
        if ((winner = same(b3, b5, b7)) != null) return winner;

        return null;
    }

    @FXML
    protected void onResetButtonClick() {
        buttonReset();
        turn = false;
        turnCount = 0;
        topLabel.setText("Kółko i Krzyrzyk");
    }

    @FXML
    protected void onGameButtonClick(javafx.event.ActionEvent event) {

        if(turn){
            Button bClicked = (Button) event.getSource();
            bClicked.setDisable(true);
            bClicked.setText("X");
            topLabel.setText("Player O turn");
            turn = false;
            turnCount++;
        }
        else{
            Button bClicked = (Button) event.getSource();
            bClicked.setDisable(true);
            bClicked.setText("O");
            topLabel.setText("Player X turn");
            turn =true;
            turnCount++;
        }

        if(checkWinner() != null){
            topLabel.setText("The winner is "+checkWinner());
            buttonDisabler();
        }

        if(turnCount>8 && checkWinner() == null){
            topLabel.setText("No contest");
            buttonDisabler();
        }

    }
}
