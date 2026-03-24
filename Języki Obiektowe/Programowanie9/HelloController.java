package com.example.programowanie9;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

import java.io.File;
import java.net.URL;
import java.util.ArrayList;

public class HelloController {
    @FXML
    private Label topLabel;
    @FXML
    private ImageView imageView;

    private int imgIndex = 0;
    private ArrayList<Image> images = new ArrayList<>();

    public void initialize() {
        try {
            URL folderUrl = getClass().getResource("/images");

            if (folderUrl == null) {
                System.out.println("Images folder not found");
                return;
            }

            File folder = new File(folderUrl.toURI());

            File[] listOfFiles = folder.listFiles();

            if (listOfFiles != null) {
                for (File file : listOfFiles) {
                    if (file.isFile() && isImageFile(file.getName())) {

                        Image img = new Image(file.toURI().toString(), 400, 400, false, false);
                        images.add(img);
                        System.out.println("Found and Loaded: " + file.getName());
                    }
                }
            }

            if (!images.isEmpty()) {
                imageView.setImage(images.get(imgIndex));
                updateLabel();
            } else {
                topLabel.setText("No images found!");
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private boolean isImageFile(String name) {
        String lowerName = name.toLowerCase();
        return lowerName.endsWith(".jpg") || lowerName.endsWith(".jpeg") ||lowerName.endsWith(".png");
    }

    @FXML
    protected void changeImg() {
        if (images.isEmpty()) return;

        if (imgIndex + 1 >= images.size()) {
            imgIndex = 0;
        } else {
            imgIndex++;
        }
        imageView.setImage(images.get(imgIndex));
        updateLabel();
    }

    private void updateLabel() {
        topLabel.setText("Obrazek " + (imgIndex + 1) + " z " + images.size());
    }
}