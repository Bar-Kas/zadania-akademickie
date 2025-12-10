import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class HelloWindow  extends JFrame {

    HelloWindow(){

        System.out.println("Konstruktor");
        setSize(960,540);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        ImageIcon icon = new ImageIcon("kruszek.jpg");

        Image img = icon.getImage();
        Image scaledImg = img.getScaledInstance(300, 400, Image.SCALE_SMOOTH);
        icon = new ImageIcon(scaledImg);

        JButton button = new JButton(icon);

        button.addActionListener(new ButtonListener());

        add(button);

        setVisible(true);


    }

}
