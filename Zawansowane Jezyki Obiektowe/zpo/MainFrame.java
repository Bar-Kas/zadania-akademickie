package com.lab.statistics;

import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.UIManager;

public class MainFrame {
    private JFrame frame;
    private static final String DIR_PATH = "files"; 
    private final int liczbaWyrazowStatystyki; 
    private final AtomicBoolean fajrant; 
    private final int liczbaProducentow; 
    private final int liczbaKonsumentow; 
    private ExecutorService executor; 
    private List<Future<?>> producentFuture; 

    public static void main(String[] args) {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName()); 
        } catch (Exception e) {
            e.printStackTrace(); 
        }
        EventQueue.invokeLater(() -> {
            try {
                MainFrame window = new MainFrame(); 
                window.frame.pack(); 
                window.frame.setAlwaysOnTop(true); 
                window.frame.setVisible(true); 
            } catch (Exception e) {
                e.printStackTrace(); 
            }
        });
    }

    public MainFrame() {
        liczbaWyrazowStatystyki = 10; 
        fajrant = new AtomicBoolean(false); 
        liczbaProducentow = 1; 
        liczbaKonsumentow = 2; 
        executor = Executors.newFixedThreadPool(liczbaProducentow + liczbaKonsumentow); 
        producentFuture = new CopyOnWriteArrayList<>(); 
        initialize(); 
    }

    private void initialize() { 
        frame = new JFrame(); 
        frame.addWindowListener(new WindowAdapter() { 
            @Override
            public void windowClosing(WindowEvent e) { 
                executor.shutdownNow(); 
            }
        });
        frame.setBounds(100, 100, 450, 300); 
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); 

        JPanel panel = new JPanel(); 
        frame.getContentPane().add(panel, BorderLayout.NORTH); 

        JButton btnStart = new JButton("Start"); 
        btnStart.addActionListener(e -> getMultiThreadedStatistics()); 

        JButton btnStop = new JButton("Stop"); 
        btnStop.addActionListener(e -> {
            fajrant.set(true); 
            for (Future<?> f : producentFuture) { 
                f.cancel(true); 
            }
        });

        JButton btnZamknij = new JButton("Zamknij"); 
        btnZamknij.addActionListener(e -> {
            executor.shutdownNow(); 
            frame.dispose(); 
        });

        panel.add(btnStart); 
        panel.add(btnStop); 
        panel.add(btnZamknij); 
    }

    private void getMultiThreadedStatistics() { 
        for (Future<?> f : producentFuture) { 
            if (!f.isDone()) { 
                JOptionPane.showMessageDialog(frame, "Nie można uruchomić nowego zadania! Przynajmniej jeden producent nadal działa!", "OSTRZEŻENIE", JOptionPane.WARNING_MESSAGE); 
                return; 
            }
        }

        fajrant.set(false); 
        producentFuture.clear(); 
        final BlockingQueue<Optional<Path>> kolejka = new LinkedBlockingQueue<>(liczbaKonsumentow); 
        final int przerwa = 60; 

        Runnable producent = () -> { 
            final String name = Thread.currentThread().getName(); 
            String info = String.format("PRODUCENT %s URUCHOMIONY .", name); 
            System.out.println(info); 

            while (!Thread.currentThread().isInterrupted()) { 
                if (fajrant.get()) { 
                    try {
                        for (int i = 0; i < liczbaKonsumentow; i++) {
                            kolejka.put(Optional.empty()); 
                        }
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt(); 
                    }
                    break;
                } else {
                    Path rootDirectory = Paths.get(DIR_PATH); 
                    if(Files.exists(rootDirectory)) {
                        try {
                            Files.walkFileTree(rootDirectory, new SimpleFileVisitor<Path>() { 
                                @Override
                                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) { 
                                    if (fajrant.get() || Thread.currentThread().isInterrupted()) {
                                        return FileVisitResult.TERMINATE;
                                    }
                                    if (file.toString().endsWith(".txt")) { 
                                        try {
                                            kolejka.put(Optional.ofNullable(file)); 
                                        } catch (InterruptedException e) {
                                            Thread.currentThread().interrupt(); 
                                            return FileVisitResult.TERMINATE;
                                        }
                                    }
                                    return FileVisitResult.CONTINUE; 
                                }
                            });
                        } catch (IOException e) {
                            System.err.println("Błąd przeszukiwania: " + e.getMessage());
                        }
                    }

                    info = String.format("Producent %s ponownie sprawdzi katalogi za %d sekund", name, przerwa); 
                    System.out.println(info); 
                    try {
                        TimeUnit.SECONDS.sleep(przerwa); 
                    } catch (InterruptedException e) { 
                        info = String.format("Przerwa producenta %s przerwana!", name); 
                        System.out.println(info); 
                        if (!fajrant.get()) Thread.currentThread().interrupt(); 
                    }
                }
            }

            info = String.format("PRODUCENT %s SKOŃCZYŁ PRACĘ", name); 
            System.out.println(info); 
        };

        Runnable konsument = () -> { 
            final String name = Thread.currentThread().getName(); 
            String info = String.format("KONSUMENT %s URUCHOMIONY", name); 
            System.out.println(info); 

            while (!Thread.currentThread().isInterrupted()) { 
                try {
                    Optional<Path> optPath = kolejka.take(); 
                    if (optPath.isPresent()) { 
                        Map<String, Long> countedWords = WordStatisticsUtil.getLinkedCountedWords(optPath.get(), liczbaWyrazowStatystyki); 
                        System.out.println("Konsument [" + name + "] - Plik: " + optPath.get().getFileName() + " -> Wynik: " + countedWords); 
                    } else {
                        break; 
                    }
                } catch (InterruptedException e) { 
                    info = String.format("Oczekiwanie konsumenta %s na nowy element z kolejki przerwane!", name); 
                    System.out.println(info); 
                    Thread.currentThread().interrupt(); 
                }
            }

            info = String.format("KONSUMENT %s ZAKOŃCZYŁ PRACĘ", name); 
            System.out.println(info); 
        };

        
        for (int i = 0; i < liczbaProducentow; i++) { 
            Future<?> pf = executor.submit(producent); 
            producentFuture.add(pf); 
        }

        
        for (int i = 0; i < liczbaKonsumentow; i++) { 
            executor.execute(konsument); 
        }
    }
}