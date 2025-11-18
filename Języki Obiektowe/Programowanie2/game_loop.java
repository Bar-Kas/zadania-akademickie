import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

public class game_loop extends JFrame {

    private int diamondCounter = 0;
    private int gamecounter = 0;
    private final JTextArea current_info = new JTextArea();
    private final JTextArea information = new JTextArea();

    private final Random rand = new Random();
    private final ImageIcon ukConIcon;
    private final ImageIcon bConIcon;
    private final ImageIcon dConIcon;

    private final Map<JButton, Timer> revertTimers = new HashMap<>();

    public game_loop() {
        setSize(800, 600);
        setTitle("Blackjack");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new GridLayout(2, 2, 10, 10));

        current_info.setText("Przetestuj swoje szczescie i kliknij na jednego z kotkow");
        information.setText("");

        ImageIcon UkCon = new ImageIcon("kruszek.jpg");
        Image UkImg = UkCon.getImage();
        Image SUkImg = UkImg.getScaledInstance(380, 280, Image.SCALE_SMOOTH);
        ukConIcon = new ImageIcon(SUkImg);

        ImageIcon BCon = new ImageIcon("DemoMan.png");
        Image BImg = BCon.getImage();
        Image SBImg = BImg.getScaledInstance(380, 280, Image.SCALE_SMOOTH);
        bConIcon = new ImageIcon(SBImg);

        ImageIcon DCon = new ImageIcon("Diamond.png");
        Image DImg = DCon.getImage();
        Image SDImg = DImg.getScaledInstance(380, 280, Image.SCALE_FAST);
        dConIcon = new ImageIcon(SDImg);

        final JButton button1 = new JButton(ukConIcon);
        button1.setBackground(new Color(255, 255, 255));

        final JButton button2 = new JButton(ukConIcon);
        button2.setBackground(new Color(255, 255, 255));



        button1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                handleButtonClick(button1, false);
            }
        });

        button2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                handleButtonClick(button2, true);
            }
        });

        add(button1);
        add(button2);
        add(current_info);
        add(information);

        setVisible(true);
    }

    private void handleButtonClick(final JButton btn, boolean reverseOutcome) {
        if (gamecounter >= 20) {
            resetGame();
            return;
        }

        gamecounter++;

        boolean win = rand.nextBoolean();
        if (reverseOutcome) {
            win = !win;
        }

        if (win) {
            diamondCounter++;
            current_info.setText("Liczba diamentów " + diamondCounter + " a liczba prob to " + gamecounter);
            information.setText("Miales farta.\nSKibidi");
            btn.setIcon(dConIcon);
            btn.setBackground(new Color(0, 170, 84));
        } else {
            current_info.setText("Liczba diamentów " + diamondCounter + " a liczba prob to " + gamecounter);
            information.setText("Trafiles na bombe.\nNie Skibidi");
            btn.setIcon(bConIcon);
            btn.setBackground(new Color(170, 0, 0));
        }

        if (gamecounter == 20) {
            information.setText("Koniec Gry\nKliknij w dowolny przycisk aby zresetowac.");
            current_info.setText("Liczba diamentów " + diamondCounter + " a liczba prob to " + gamecounter);
        }

        cancelRevertTimer(btn);
        Timer revertTimer = new Timer(1500, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                btn.setIcon(ukConIcon);
                btn.setBackground(new Color(255, 255, 255));
                revertTimers.remove(btn);
            }
        });
        revertTimer.setRepeats(false);
        revertTimer.start();
        revertTimers.put(btn, revertTimer);
    }

    private void cancelRevertTimer(JButton btn) {
        if (btn == null) return;
        Timer t = revertTimers.remove(btn);
        if (t != null) {
            t.stop();
        }
    }

    private void resetGame() {
        diamondCounter = 0;
        gamecounter = 0;

        for (Timer t : revertTimers.values()) {
            if (t != null) t.stop();
        }
        revertTimers.clear();

        current_info.setText("Liczba diamentów " + diamondCounter + " a liczba prob to " + gamecounter);
        information.setText("Wszystkie statystyki zostały wyzerowane.");
    }

    static void main() {
        SwingUtilities.invokeLater(game_loop::new);
    }
}
